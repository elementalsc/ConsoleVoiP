#include "UdpAudioServer.h"
#include "UdpAudioSession.h"

UdpAudioServer::UdpAudioServer(boost::asio::io_service & service, short int port) :
  mSocket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
  mUdpAudioSessionCount(0)
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
        std::make_shared<UdpAudioSession>(std::move(mSocket), std::move(mUdpAudioSessionCount++))->start();
      }
      accept();
    });
}