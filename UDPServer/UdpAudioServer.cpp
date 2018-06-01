#include "UdpAudioServer.h"
#include "../Audio/AudioStream.h"

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

        // Check if it's already in the map
        mapIterator mapIt = mAudioStreamsMap.find(wIpAddress);

        // If it is not
        if(mapIt == mAudioStreamsMap.end())
        {
          // Create a new AudioStream for this specific address
          mapEntry wNewMapEntry (wIpAddress, std::make_shared<AudioStream>());
          wNewMapEntry.second->start();

          // Add it to the map
          {
            std::lock_guard<std::mutex> lock(mMapMutex);
            mapIt = mAudioStreamsMap.insert(mAudioStreamsMap.end(), std::move(wNewMapEntry));
          }
        }

        // Feed received audio to stream
        mapIt->second->feed(reinterpret_cast<char*>(mData), iBytesReceived/2);
      }

      // Validate if this is necessary
      accept();
    });
}