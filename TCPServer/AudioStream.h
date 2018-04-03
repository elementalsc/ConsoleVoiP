#include <mutex>
#include <SFML/Audio.hpp>


class AudioStream : public sf::SoundStream
{
public:
  AudioStream();
  void AudioStream::feed(const char* iCharSamples, unsigned int iSampleCount);
  void AudioStream::start();
private:

  bool AudioStream::onGetData(sf::SoundStream::Chunk& data);
  void AudioStream::onSeek(sf::Time timeOffset);
  void AudioStream::receiveLoop();
  
  std::mutex              m_mutex;
  std::vector<sf::Int16> m_samples;
  std::vector<sf::Int16> m_tempBuffer;
  std::size_t            m_offset;
  bool                   m_hasFinished;

};