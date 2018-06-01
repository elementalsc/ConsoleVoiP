#include <iostream>
#include <mutex>
#include <string>
#include <regex>
#include <boost/asio.hpp>
#include <SFML/Audio.hpp>

using namespace boost;
using namespace boost::asio::ip;

class TcpDestination
{
public:
  TcpDestination(std::string iHost, int iPort) :
    mHost(iHost),
    mPort(iPort)
  {
    address_v4 wIpAddress = address_v4::from_string(iHost);
    mEndpoint = tcp::endpoint(wIpAddress, iPort);
  }

  tcp::endpoint getEndpoint() { return mEndpoint;}
  std::string   getHost()     { return mHost;}
  int           getPort()     { return mPort;}

private:
  tcp::endpoint mEndpoint;
  std::string mHost;
  int mPort;
};



class TcpAudioClient : public sf::SoundRecorder
{
public:
  TcpAudioClient();
  ~TcpAudioClient();

  bool init(std::string iHost, std::string iPort);
  void addDestination(std::string iHost, int iPort);
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

  std::vector<std::shared_ptr<TcpDestination>>  destinationList;
  std::vector<std::shared_ptr<tcp::socket>>     socketList;

  asio::io_service mService;
  tcp::socket*   mSocket;
  tcp::resolver* mResolver;
  boost::system::error_code ec;
};