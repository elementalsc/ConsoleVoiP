#include "AudioStream.h"

AudioStream::AudioStream() :
  mOffset(0)
{
  // Set the sound parameters
  initialize(1, 44100);
  std::cout << "AudioStream initialized!" << std::endl;
  play();
}

void AudioStream::start()
{
  play();
}

bool AudioStream::onGetData(sf::SoundStream::Chunk& data)
{

  // No new data has arrived since last update: wait until we get some
  while (mOffset >= mSamples.size())
  {
    sf::sleep(sf::milliseconds(20));
  }

  // Copy samples into a local buffer to avoid synchronization problems
  {
    std::lock_guard<std::mutex> lock(mSamplesMutex);
    mTempBuffer.assign(mSamples.begin() + mOffset, mSamples.end());
  }

  // Fill audio data to pass to the stream
  data.samples = &mTempBuffer[0];
  data.sampleCount = mTempBuffer.size();

  // Update the playing offset
  mOffset += mTempBuffer.size();

  return true;
}

void AudioStream::onSeek(sf::Time timeOffset)
{
  mOffset = timeOffset.asMilliseconds() * getSampleRate() * getChannelCount() / 1000;
}

void AudioStream::feed(const char* iCharSamples, std::size_t iSampleCount)
{
  // Convert data from uint8 to samples format
  const sf::Int16* wSamples = reinterpret_cast<const sf::Int16*>(iCharSamples);

  // Copy new samples to class buffer
  {
    std::lock_guard<std::mutex> lock(mSamplesMutex);
    std::copy(wSamples, wSamples + iSampleCount, std::back_inserter(mSamples));
  }
}

void AudioStream::feed(const sf::Int16* iSamples, std::size_t iSampleCount)
{
  // Copy new samples to class buffer
  {
    std::lock_guard<std::mutex> lock(mSamplesMutex);
    std::copy(iSamples, iSamples + iSampleCount, std::back_inserter(mSamples));
  }
}