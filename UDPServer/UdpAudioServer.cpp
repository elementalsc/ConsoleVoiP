#include "UdpAudioServer.h"


UdpAudioServer::UdpAudioServer(boost::asio::io_service & service, short int port) :
  mSocket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))

{
  accept();
}

UdpAudioServer::~UdpAudioServer()
{
}

void
UdpAudioServer::accept()
{
  mSocket.async_receive_from(boost::asio::buffer(mData), mClientEndpoint, 
    [this](boost::system::error_code ec, std::size_t iBytesReceived)
    {
      if(iBytesReceived)
      {
        std::string wString(mData, iBytesReceived) ;
        std::cout << "Server received " << iBytesReceived << " bytes from " << mClientEndpoint.address().to_string() + " : " << wString << std::endl;
      }
      accept();
    });
}