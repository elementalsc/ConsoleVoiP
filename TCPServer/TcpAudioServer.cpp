#include "TcpAudioServer.h"

TcpAudioServer::TcpAudioServer(boost::asio::io_service & service, const short port) : 
  mAcceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), 
  mSocket(service),
  mTcpAudioSessionCount(0)
{
  mAcceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address());
  accept();
}

void TcpAudioServer::accept() 
{
  mAcceptor.async_accept(mSocket, 
    [this](boost::system::error_code ec) 
  {
    if (!ec) 
    {
      std::make_shared<TcpAudioSession>(std::move(mSocket), mTcpAudioSessionCount++)->start();
    }

    accept();
  });
}