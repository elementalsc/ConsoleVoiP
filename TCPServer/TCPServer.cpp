#include "TcpAudioServer.h"
#include <thread>

int main()
{
  int wPort = 2112;

  boost::asio::io_service ioService;

  std::cout << "Starting server on port " << wPort << "..." << std::endl;

  TcpAudioServer mTcpAudioServer(ioService, wPort);

  std::thread serviceThread([&](){ioService.run();}); 

  serviceThread.join();
}

