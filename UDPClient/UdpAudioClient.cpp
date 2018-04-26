#include "UdpAudioClient.h"


UdpAudioClient::UdpAudioClient() :
  mEnableSending(false),
  mDebouncingTime(125)
{
  // get the available sound input device names
  std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
  // 
  //   std::cout << "Select input device..." << std::endl;
  // 

  std::cout << "Available devices : " << std::endl;

  for(unsigned int i = 0; i < availableDevices.size(); ++i)
  {
    std::cout << i << " : " << availableDevices[i] << std::endl;
  }
  // 
  //   std::string iInputDevice;
  //    std::cin >> iInputDevice;
  //   // choose a device
  // 
  //   // set the device
  //   if (!setDevice(availableDevices[stoi(iInputDevice)]))
  if (!setDevice(availableDevices[0]))
  {
    std::cout << "An error occured!" << std::endl;
  }

  setProcessingInterval(sf::milliseconds(20));
}

UdpAudioClient::~UdpAudioClient()
{
    if(!mEndpoint) delete mEndpoint;
    if(!mSocket)
    {
      mSocket->close();
      delete mSocket;
    }
}

bool UdpAudioClient::init(std::string & iHost, std::string & iPort)
{
  if(!std::regex_match(iHost,std::regex("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}")))
  {
    std::cout << "Host address invalid!" << std::endl;
    return false;
  }

  if(!std::regex_match(iPort,std::regex("\\d{1,5}")))
  { 
    std::cout << "Port number invalid!" << std::endl;
    return false;
  }

  mHost = iHost;
  mPort = stoi(iPort);

  try
  {
    std::lock_guard<std::mutex> lock(mSocketMutex);
    boost::asio::ip::address wIpAddress = boost::asio::ip::address::from_string(mHost);
    mEndpoint = new boost::asio::ip::udp::endpoint(wIpAddress, mPort);
    mSocket = new boost::asio::ip::udp::socket(mService);
    mSocket->open(boost::asio::ip::udp::v4());
  } 
  catch (std::exception& e)
  {
    std::cerr << "Exception :" << std::endl << e.what() << "\n";
    return false;
  }

  std::cout << std::endl << "Successfully created socket and endpoint to " << mEndpoint->address().to_string() << " on port " << mEndpoint->port() << std::endl;
  return true;
}

// void UdpAudioClient::send(std::string & iTextToSend)
// {
//   //iTextToSend.append('\0');
//   mSocket->send_to(boost::asio::buffer(iTextToSend),*mEndpoint);
// 
// }




bool UdpAudioClient::onStart() // optional
{
  std::cout << "Started UDPAudioClient" << std::endl;
  return true;
}

void UdpAudioClient::onStop() // optional
{
  std::cout << "Stopped UDPAudioClient" << std::endl;
}




//=================================================================================================
//
//  REAL PLEASURE BELOW!
//
//=================================================================================================


bool UdpAudioClient::send(bool iEnableSending)
{

  // Enable
  if(iEnableSending)
  {
    
     // Check actual state of switch
    if(mEnableSending) return true;



    // Debouncing
    Sleep(mDebouncingTime);

    // Wait for recorder to become available
    while(!isAvailable())
    {
      Sleep(10);
    }

    // Start recording
    start();

    std::cout << std::endl << "Recording started..." << std::endl;
    mEnableSending = true;
    return true;
  }

  // Disable
  else
  {
    // Check actual state of switch
    if(!mEnableSending) return true;

    // Debouncing
    Sleep(mDebouncingTime);

    // Stop recording
    stop();
    mEnableSending = false;
    return true;
  }
}

bool UdpAudioClient::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
  // convert to chars
  unsigned int  samplesAsCharsCount = sampleCount*2;
  char *        samplesAsChars      = new char[samplesAsCharsCount]();

  samplesAsChars = reinterpret_cast<char*>(const_cast<sf::Int16*>(samples));

  // EnableSending gate
  if(!mEnableSending) return false;

  // Send over UDP
  try
  {
    std::lock_guard<std::mutex> lock(mSocketMutex);
    if(mSocket)
    {

      mSocket->send_to(boost::asio::buffer(samplesAsChars,samplesAsCharsCount),*mEndpoint, 0, ec);
      std::cout << ">";
    }
  } 
  catch (std::exception& e)
  {
    std::cout << "boost::asio error : " << ec.message() << std::endl;
    std::cerr << "Exception :" << std::endl << e.what() << "\n";
  }

  // Continue processing
  return true;
}