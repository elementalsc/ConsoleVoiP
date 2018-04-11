#include <iostream>
#include <boost/asio.hpp>

#define BUFFER_SIZE 4096

class UdpAudioServer
{
public:
  UdpAudioServer(boost::asio::io_service & service, short int port);
  ~UdpAudioServer();
  void accept();

private:


  boost::asio::ip::udp::socket mSocket;
  unsigned int mUdpAudioSessionCount;
  char mData[BUFFER_SIZE];
  boost::asio::ip::udp::endpoint mClientEndpoint; 
};