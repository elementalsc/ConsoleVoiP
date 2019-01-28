#pragma once
#include <mutex>
#include <vector>
#include <iostream>
#include <SFML/Audio.hpp>

class AudioStream : public sf::SoundStream
{
public:
  AudioStream();

  void AudioStream::feed(const char* iCharSamples, std::size_t iSampleCount);
  void AudioStream::feed(const sf::Int16* iSamples, std::size_t iSampleCount);
  void AudioStream::start();

private:
  bool AudioStream::onGetData(sf::SoundStream::Chunk& data);
  void AudioStream::onSeek(sf::Time timeOffset);

  std::mutex             mSamplesMutex;
  std::vector<sf::Int16> mSamples;
  std::vector<sf::Int16> mTempBuffer;
  std::size_t            mOffset;
};