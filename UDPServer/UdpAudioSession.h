#include <vector>
#include <iostream>
#include <boost/asio.hpp>
#include "../Audio/AudioStream.h"

#define BUFFER_SIZE 4096

class UdpAudioSession : public std::enable_shared_from_this<UdpAudioSession> 
{    
public:
  UdpAudioSession(boost::asio::ip::udp::socket && iSocket, unsigned int && iUdpAudioSessionId);
  ~UdpAudioSession();

  void start();

private:
  void doRead();

  uint8_t mData[BUFFER_SIZE];
  AudioStream mAudioStream;
  unsigned int mUdpAudioSessionId;
  boost::asio::ip::udp::socket mSocket;
  boost::asio::ip::udp::endpoint mEndpoint;
};