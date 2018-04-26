#include <iostream>
#include <mutex>
#include <string>
#include <regex>
#include <boost/asio.hpp>
#include <SFML/Audio.hpp>

using namespace boost;
using namespace boost::asio::ip;

class TcpAudioClient : public sf::SoundRecorder
{
public:
  TcpAudioClient();
  ~TcpAudioClient();

  bool init(std::string iHost, std::string iPort);
  bool send(bool iEnableSending);

private:

  bool onStart();
  void onStop();
  bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);

  bool mEnableSending;
  std::string mHost;
  int mPort;
  std::mutex mSocketMutex;
  unsigned int mDebouncingTime;

  asio::io_service mService;
  tcp::socket*   mSocket;
  tcp::resolver* mResolver;
  boost::system::error_code ec;
};