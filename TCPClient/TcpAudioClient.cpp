#include "TcpAudioClient.h"


TcpAudioClient::TcpAudioClient() : mEnableSending(false)
{
   // get the available sound input device names
   std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
// 
//   std::cout << "Select input device..." << std::endl;
// 
//   for(unsigned int i = 0; i < availableDevices.size(); ++i)
//   {
//     std::cout << i << " : " << availableDevices[i] << std::endl;
//   }
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

TcpAudioClient::~TcpAudioClient()
{
  std::lock_guard<std::mutex> lock(mSocketMutex);
  mSocket->shutdown(tcp::socket::shutdown_both, ec);
  mSocket->close();
  if(!mSocket)    delete mSocket;
  if(!mResolver)  delete mResolver;
  if(!mQuery)     delete mQuery;
}



bool TcpAudioClient::init(std::string iHost, std::string iPort)
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
  mPort = iPort;
  return true;
}


bool TcpAudioClient::broadcast(bool iEnableSending)
{

  // Enable
  if(iEnableSending)
  {
    // Check actual state of switch
    if(mEnableSending) return true;

    // Open connection
    try
    {
      std::lock_guard<std::mutex> lock(mSocketMutex);
      mSocket = new tcp::socket(mService);
      mResolver = new tcp::resolver(mService);
      mQuery = new tcp::resolver::query(mHost, mPort);
      connect(*mSocket, mResolver->resolve(*mQuery),ec);
    } 
    catch (std::exception& e)
    {
      std::cerr << "Exception :" << std::endl << e.what() << "\n";
      return false;
    }


    std::cout << std::endl << "Successfully opened connection!" << std::endl;

    // Start recording
    start();
    mEnableSending = true;
    return true;
  }

  // Disable
  else
  {
    // Check actual state of switch
    if(!mEnableSending) return true;
    
    // Close connection
    try
    {      
      std::lock_guard<std::mutex> lock(mSocketMutex);
      mSocket->shutdown(tcp::socket::shutdown_both, ec);
      mSocket->close();
      if(!mSocket)    delete mSocket;
      if(!mResolver)  delete mResolver;
      if(!mQuery)     delete mQuery;
    } 
    catch (std::exception& e)
    {
      std::cerr << "Exception :" << std::endl << e.what() << "\n";
      return false;
    }


    std::cout << std::endl << "Successfully closed connection!" << std::endl;

    // Stop recording
    stop();
    mEnableSending = false;
    
    return true;
  }
}

bool TcpAudioClient::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
  // convert to chars
  unsigned int  samplesAsCharsCount = sampleCount*2;
  char *        samplesAsChars      = new char[samplesAsCharsCount]();
  
  samplesAsChars = reinterpret_cast<char*>(const_cast<sf::Int16*>(samples));

  // EnableSending gate
  if(!mEnableSending) return false;
  // Send over TCP
  try
  {
    std::lock_guard<std::mutex> lock(mSocketMutex);
    if(mSocket)
    {
      write(*mSocket, boost::asio::buffer(samplesAsChars,samplesAsCharsCount),ec);
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



bool TcpAudioClient::onStart() // optional
{
  std::cout << "Started recording :D" << std::endl;
  return true;
}

void TcpAudioClient::onStop() // optional
{
  std::cout << "Recording stopped :(" << std::endl;
}