#include "TcpAudioSession.h"

TcpAudioSession::TcpAudioSession(boost::asio::ip::tcp::socket &&iSocket, unsigned int && iTcpAudioSessionId) : 
  mSocket(std::move(iSocket)), 
  mTcpAudioSessionId(iTcpAudioSessionId) 
{
  mAudioStream.start();
  std::cout << "TcpAudioSession " << mTcpAudioSessionId << " started :  " 
            << mSocket.local_endpoint().address().to_string() 
            << ":" << mSocket.local_endpoint().port() << std::endl;
}

TcpAudioSession::~TcpAudioSession()
{
  mAudioStream.stop();
  std::cout << "TcpAudioSession " << mTcpAudioSessionId << " closed  :  " 
            << mSocket.local_endpoint().address().to_string() << ":" 
            << mSocket.local_endpoint().port() << std::endl;
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