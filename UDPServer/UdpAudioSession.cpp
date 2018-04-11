#include "UdpAudioSession.h"

UdpAudioSession::UdpAudioSession(boost::asio::ip::udp::socket &&iSocket, unsigned int && iUdpAudioSessionId) : 
  mSocket(std::move(iSocket)), 
  mUdpAudioSessionId(iUdpAudioSessionId) 
{
  mAudioStream.start();
  std::cout << "UdpAudioSession " << mUdpAudioSessionId << " started :  " 
    << mSocket.local_endpoint().address().to_string() 
    << ":" << mSocket.local_endpoint().port() << std::endl;
}

UdpAudioSession::~UdpAudioSession()
{
  mAudioStream.stop();
  std::cout << "UdpAudioSession " << mUdpAudioSessionId << " closed  :  " 
    << mSocket.local_endpoint().address().to_string() << ":" 
    << mSocket.local_endpoint().port() << std::endl;
}

void UdpAudioSession::start() 
{ 
  doRead(); 
}

void UdpAudioSession::doRead() 
{
  std::shared_ptr<UdpAudioSession> self(shared_from_this());

  mSocket.async_receive_from(
    boost::asio::buffer(mData, BUFFER_SIZE), mEndpoint,
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