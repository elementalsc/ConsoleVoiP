#include <mutex>
#include <deque>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <boost/asio.hpp>
#include <SFML/Audio.hpp>
#include "AudioStream.h"

#define PORT 2112
#define BUFFER_SIZE 4096

////////////////////////////////////////////////////////////////////////////////////////////////////////
//      SERVER 
////////////////////////////////////////////////////////////////////////////////////////////////////////

class TcpAudioServer 
{
public:
  // Ctor
  TcpAudioServer(boost::asio::io_service &iService, const short iPort);

private:
  // Methods
  void doAccept();

  // Variables
  boost::asio::ip::tcp::acceptor mAcceptor;
  boost::asio::ip::tcp::socket mSocket;
  unsigned int mTcpAudioSessionCount;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//      SESSION
////////////////////////////////////////////////////////////////////////////////////////////////////////


class TcpAudioSession : public std::enable_shared_from_this<TcpAudioSession> 
{    
public:
  // Ctor
  TcpAudioSession(boost::asio::ip::tcp::socket && iSocket, unsigned int && iTcpAudioSessionId);
  ~TcpAudioSession();

  // Methods
  void start();

private:
  // Methods
  void doRead();

  // Variables
  boost::asio::ip::tcp::socket mSocket;
  uint8_t mData[BUFFER_SIZE];

  AudioStream mAudioStream;

  unsigned int mTcpAudioSessionId;
  mutable std::mutex mTcpAudioSessionMutex;
  std::deque<std::tuple<size_t,unsigned char*>> mTcpAudioSessionChunkQueue;
};
