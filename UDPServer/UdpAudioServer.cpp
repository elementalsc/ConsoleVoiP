#include "UdpAudioServer.h"
#include "AudioStream.h"

UdpAudioServer::UdpAudioServer(boost::asio::io_service & service, short int port) :
  mSocket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
  mUdpAudioSessionCount(0)
{
  accept();
}

UdpAudioServer::~UdpAudioServer()
{
  // Clean AudioStreamMap
}

void
UdpAudioServer::accept()
{
  mSocket.async_receive_from(boost::asio::buffer(mData), mClientEndpoint, 
    [this](boost::system::error_code ec, std::size_t iBytesReceived)
    {      
      if (ec)
      {
        return;
      }

      if(iBytesReceived)
      {
        // Get sender's IP address
        std::string wIpAddress(mClientEndpoint.address().to_string());

        // If it does not exist
        if(mAudioStreamsMap.find(wIpAddress) == mAudioStreamsMap.end())
        {
          std::lock_guard<std::mutex> lock(mMapMutex);
          // Create a new AudioStream for this specific address
          mAudioStreamsMap[wIpAddress] = std::make_shared<AudioStream>();
          mAudioStreamsMap[wIpAddress]->start();
        }

        // Feed received audio to stream
        mAudioStreamsMap[wIpAddress]->feed(reinterpret_cast<char*>(mData), iBytesReceived/2);
      }

      // Validate if this is necessary
      accept();
    });
}