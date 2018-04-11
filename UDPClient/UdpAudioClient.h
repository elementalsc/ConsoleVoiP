#include <iostream>
#include <mutex>
#include <string>
#include <regex>
#include <boost/asio.hpp>
#include <SFML/Audio.hpp>

class UdpAudioClient
{
public:

  UdpAudioClient();
  ~UdpAudioClient();
  
  bool init(std::string & iHost, std::string & iPort);
  void send(std::string & iTextToSend);

private:

  boost::asio::io_service mService;
  boost::asio::ip::udp::socket* mSocket;
  boost::asio::ip::udp::endpoint* mEndpoint;
  short unsigned int mPort;
  std::string mHost;

};