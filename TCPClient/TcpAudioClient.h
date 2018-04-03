#include <iostream>
#include <mutex>
#include <thread>
#include <SFML/Audio.hpp>
#include <string>
#include <deque>
#include <boost/asio.hpp>
#include <regex>

using namespace boost;
using namespace boost::asio::ip;

class TcpAudioClient : public sf::SoundRecorder
{
public:
  TcpAudioClient();
  ~TcpAudioClient();

  bool init(std::string iHost, std::string iPort);
  bool broadcast(bool iEnableSending);

private:

  bool onStart();
  void onStop();
  bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);

  bool mEnableSending;

  // setup tcp ip variables
  std::string mHost;// = "localhost";
  std::string mPort;// = "2112";
  asio::io_service    mService;
  tcp::socket*   mSocket;
  tcp::resolver* mResolver;
  tcp::resolver::query*  mQuery;
  boost::system::error_code ec;

  std::mutex mSocketMutex;



};