#include "UdpAudioServer.h"
#include <thread>

int main()
{
  int wPort = 2112;

  boost::asio::io_service ioService;

  std::cout << "Starting server on port " << wPort << "..." << std::endl;

  // Creating instance of UDP server
  UdpAudioServer mUdpAudioServer(ioService, wPort);

  // Run its service
  std::thread serviceThread([&](){ioService.run();}); 

  serviceThread.join();
}

