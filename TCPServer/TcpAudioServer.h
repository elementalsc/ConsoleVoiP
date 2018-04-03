#include <iostream>
#include <boost/asio.hpp>
#include "TcpAudioSession.h"

class TcpAudioServer 
{
public:
  TcpAudioServer(boost::asio::io_service &iService, const short iPort);

private:
  void doAccept();

  boost::asio::ip::tcp::acceptor mAcceptor;
  boost::asio::ip::tcp::socket mSocket;
  unsigned int mTcpAudioSessionCount;
};