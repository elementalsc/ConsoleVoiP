#include "TcpAudioServer.h"

TcpAudioServer::TcpAudioServer(boost::asio::io_service & service, const short port) : 
  mAcceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), 
  mSocket(service),
  mTcpAudioSessionCount(0)
{
  mAcceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address());
  doAccept();
}

void TcpAudioServer::doAccept() 
{
  mAcceptor.async_accept(mSocket, 
    [this](boost::system::error_code error) 
  {
    if (!error) 
    {
      std::make_shared<TcpAudioSession>(std::move(mSocket), std::move(mTcpAudioSessionCount++))->start();
    }
    doAccept();
  });
}