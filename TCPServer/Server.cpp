#include "TcpAudioServer.h"


int main()
{
  boost::asio::io_service ioService;

  std::cout << "Starting server on port " << PORT << "..." << std::endl;

  TcpAudioServer mTcpAudioServer(ioService, PORT);

  std::thread serviceThread([&](){ioService.run();}); 

  serviceThread.join();
}

