#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>
#include <mutex>
//#include "../Audio/AudioStream.h"

#define BUFFER_SIZE 4096
class AudioStream;
typedef std::unordered_map<std::string, std::shared_ptr<AudioStream>>::iterator mapIterator;
typedef std::pair<std::string, std::shared_ptr<AudioStream>> mapEntry;

class UdpAudioServer
{
public:
  UdpAudioServer(boost::asio::io_service & service, short int port);
  ~UdpAudioServer();
  void accept();

private:
  boost::asio::ip::udp::socket mSocket;
  unsigned int mUdpAudioSessionCount;
  char mData[BUFFER_SIZE];
  boost::asio::ip::udp::endpoint mClientEndpoint; 
  std::mutex mMapMutex;
  std::unordered_map<std::string, std::shared_ptr<AudioStream>> mAudioStreamsMap; 

};