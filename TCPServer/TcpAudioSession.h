#include <vector>
#include <iostream>
#include <boost/asio.hpp>
#include "AudioStream.h"

#define BUFFER_SIZE 4096

class TcpAudioSession : public std::enable_shared_from_this<TcpAudioSession> 
{    
public:
  TcpAudioSession(boost::asio::ip::tcp::socket && iSocket, unsigned int && iTcpAudioSessionId);
  ~TcpAudioSession();

  void start();

private:
  void doRead();

  uint8_t mData[BUFFER_SIZE];
  AudioStream mAudioStream;
  unsigned int mTcpAudioSessionId;
  boost::asio::ip::tcp::socket mSocket;
};