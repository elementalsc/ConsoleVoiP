#include <iostream>
#include <mutex>
#include <string>
#include <regex>
#include <boost/asio.hpp>
#include <SFML/Audio.hpp>

class UdpAudioClient : public sf::SoundRecorder
{
public:

  UdpAudioClient();
  ~UdpAudioClient();
  
  bool init(std::string & iHost, std::string & iPort);
  //void send(std::string & iTextToSend);
  bool send(bool iEnableSending);

private:

  boost::asio::io_service mService;
  boost::asio::ip::udp::socket* mSocket;
  boost::asio::ip::udp::endpoint* mEndpoint;
  short unsigned int mPort;
  std::string mHost;
  boost::system::error_code ec;

  bool mEnableSending;
  std::mutex mSocketMutex;
  unsigned int mDebouncingTime;

  // SFML SoundStream methods
  bool onStart();
  void onStop();
  bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);

};