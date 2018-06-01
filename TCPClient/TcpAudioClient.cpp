#include "TcpAudioClient.h"


TcpAudioClient::TcpAudioClient() : 
  mEnableSending(false),
  mDebouncingTime(125)
{

  mResolver = new tcp::resolver(mService);

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
   else
   {
     std::cout << "Capture device selected : " << availableDevices[0] << std::endl;
     setProcessingInterval(sf::milliseconds(20));
   }
}

TcpAudioClient::~TcpAudioClient()
{
  std::lock_guard<std::mutex> lock(mSocketMutex);
  if(mSocket->is_open())
  {
    mSocket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    mSocket->close();
  }
  if(!mSocket)    delete mSocket;
  if(!mResolver)  delete mResolver;
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
  mPort = stoi(iPort);

  addDestination(mHost,mPort);
  return true;
}

void TcpAudioClient::addDestination(std::string iHost, int iPort)
{
  destinationList.push_back(std::make_shared<TcpDestination>(iHost, iPort));
}


bool TcpAudioClient::send(bool iEnableSending)
{

  // Enable
  if(iEnableSending)
  {
    // Check actual state of switch
    if(mEnableSending) return true;

    // Debouncing
    Sleep(mDebouncingTime);

    // Open connections
    try
    {
      for(std::shared_ptr<TcpDestination> wDestination : destinationList)
      {
        // Resolve distant endpoint
        tcp::resolver::iterator wEndpointIterator = mResolver->resolve(wDestination->getEndpoint());
        
        // Create and connect new socket
        // C++14 required to get all features related to unique_ptr
        socketList.push_back(std::make_shared<tcp::socket>(mService));
        connect(*socketList.back(), wEndpointIterator, ec);

        std::cout << "Connection made with " << wDestination->getHost() << ":" << wDestination->getPort() << std::endl;
      }
    } 
    catch (std::exception& e)
    {
      std::cerr << "Exception :" << std::endl << e.what() << "\n";
      return false;
    }
    

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

    // Close connection

    {
      std::lock_guard<std::mutex> lock(mSocketMutex);
      try
      {
        for(std::shared_ptr<tcp::socket> wSocket : socketList)
        {
          // If socket is already closed
          if(!wSocket->is_open()) continue;
          
          std::string wHost = wSocket->remote_endpoint().address().to_string();
          unsigned int wPort = wSocket->remote_endpoint().port();

          // Close socket
          wSocket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
          wSocket->close();
          std::cout << std::endl << "Successfully closed connection to "<< wHost << ":" << wPort << std::endl;
        }

        // Clear the vector
        socketList.clear();
      } 
      catch (std::exception& e)
      {
        std::cerr << "Could not close a connection properly" << std::endl;
        std::cerr << "Exception :" << std::endl << e.what() << "\n";

        return false;
      }

    }


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
  // 
  //  This whole Try/Catch might go in a thread!
  //
  try
  {
    std::lock_guard<std::mutex> lock(mSocketMutex);
    for(std::shared_ptr<tcp::socket> wSocket : socketList)
    {
      if(wSocket)
      {
        // Separate thread to continuously write on all sockets might be required
        
        write(*wSocket, boost::asio::buffer(samplesAsChars,samplesAsCharsCount),ec);
        std::cout << ">";
      }
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
  std::cout << "Started TCPAudioClient" << std::endl;
  return true;
}

void TcpAudioClient::onStop() // optional
{
  std::cout << "Stopped TCPAudioClient" << std::endl;
}