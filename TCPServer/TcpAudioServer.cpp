#include "TcpAudioServer.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////
//      SERVER
////////////////////////////////////////////////////////////////////////////////////////////////////////

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



////////////////////////////////////////////////////////////////////////////////////////////////////////
//      SESSION 
////////////////////////////////////////////////////////////////////////////////////////////////////////

TcpAudioSession::TcpAudioSession(boost::asio::ip::tcp::socket &&iSocket, unsigned int && iTcpAudioSessionId) : 
  mSocket(std::move(iSocket)), 
  mTcpAudioSessionId(iTcpAudioSessionId) 
{
  mAudioStream.start();
  std::cout << "TcpAudioSession " << mTcpAudioSessionId << " started :  " << mSocket.local_endpoint().address().to_string() << ":" << mSocket.local_endpoint().port() << std::endl;
}

TcpAudioSession::~TcpAudioSession()
{
  mAudioStream.stop();
  std::cout << "TcpAudioSession " << mTcpAudioSessionId << " closed  :  " << mSocket.local_endpoint().address().to_string() << ":" << mSocket.local_endpoint().port() << std::endl;
}

void TcpAudioSession::start() 
{ 
  doRead(); 
}

void TcpAudioSession::doRead() 
{
  std::shared_ptr<TcpAudioSession> self(shared_from_this());

  mSocket.async_read_some(
    boost::asio::buffer(mData, BUFFER_SIZE),
    [this, self](boost::system::error_code error, size_t iBytesReceived) 
  {
    if (error)
    {
      return;
    }

    doRead();

    if(iBytesReceived > 0) 
    {
      mAudioStream.feed(reinterpret_cast<char*>(mData), iBytesReceived/2);
    }
  });
}





