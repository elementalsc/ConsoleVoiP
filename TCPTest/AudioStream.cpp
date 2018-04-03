
#include "AudioStream.h"
#include <iostream>

AudioStream::AudioStream() :
  m_offset     (0),
  m_hasFinished(false)
{
  // Set the sound parameters
  initialize(1, 44100);
  std::cout << "AudioStream initialized!" << std::endl;
}

void AudioStream::start()
{
  play();
//   if (!m_hasFinished)
//   {
//     // Start playback
//     play();
// 
//     // Start receiving audio data
//     receiveLoop();
//   }
//   else
//   {
//     // Start playback
//     play();
//   }
}


bool AudioStream::onGetData(sf::SoundStream::Chunk& data)
{
  // We have reached the end of the buffer and all audio data have been played: we can stop playback
  if ((m_offset >= m_samples.size()) && m_hasFinished)
    return false;

  // No new data has arrived since last update: wait until we get some
  while ((m_offset >= m_samples.size()) && !m_hasFinished)
    sf::sleep(sf::milliseconds(10));

  // Copy samples into a local buffer to avoid synchronization problems
  // (don't forget that we run in two separate threads)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_tempBuffer.assign(m_samples.begin() + m_offset, m_samples.end());
  }


  // je crois que c'est en ajoutant du stock à data que le stream considère qu'un reçoit un chunk
  // Fill audio data to pass to the stream
  data.samples     = &m_tempBuffer[0];
  data.sampleCount = m_tempBuffer.size();

  // Update the playing offset
  m_offset += m_tempBuffer.size();

  return true;
}


void AudioStream::onSeek(sf::Time timeOffset)
{
  m_offset = timeOffset.asMilliseconds() * getSampleRate() * getChannelCount() / 1000;
}

void AudioStream::feed(const char* iCharSamples, unsigned int iSampleCount)
{

  const sf::Int16* samples     = reinterpret_cast<const sf::Int16*>(iCharSamples);
  std::size_t      sampleCount = iSampleCount;

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::copy(samples, samples + sampleCount, std::back_inserter(m_samples));
  }

}
void AudioStream::receiveLoop()
{
//   while (!m_hasFinished)
//   {
//     // Get waiting audio data from the network
// 
//     // Extract audio samples from the packet, and append it to our samples buffer
//     const sf::Int16* samples     = reinterpret_cast<const sf::Int16*>(static_cast<const char*>(packet.getData()) + 1);
//    std::size_t      sampleCount = (packet.getDataSize() - 1) / sizeof(sf::Int16);
// 
//     // Don't forget that the other thread can access the sample array at any time
//     // (so we protect any operation on it with the mutex)
//     {
//       std::lock_guard<std::mutex> lock(m_mutex);
//       std::copy(samples, samples + sampleCount, std::back_inserter(m_samples));
//     }
//   }
}

