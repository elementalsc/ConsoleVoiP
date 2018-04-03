#include <iostream>
#include <mutex>
#include <thread>
#include <SFML/Audio.hpp>
#include <string>
#include "../TCPTest/AudioStream.h"
#include <deque>

class AudioRecorder : public sf::SoundRecorder
{
public:
   AudioRecorder();
   //void AudioRecorder::playLiveSound();
private:
  bool mLivePlaying;

  bool onStart();
  void onStop();
  bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
  //void pushToBufferQueue(sf::SoundBuffer iSoundBuffer);


  std::deque<sf::Sound> mSoundQueue;
  std::deque<sf::SoundBuffer> mSoundBufferQueue;
  std::mutex mQueueMutex;
  AudioStream mAudioStream;

};