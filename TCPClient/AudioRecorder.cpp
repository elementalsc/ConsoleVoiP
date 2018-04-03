#include "AudioRecorder.h"

//Ctor
AudioRecorder::AudioRecorder() : mLivePlaying(false)
{
  // get the available sound input device names
  std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();

  std::cout << "Select input device..." << std::endl;

  for(unsigned int i = 0; i < availableDevices.size(); ++i)
  {
    std::cout << i << " : " << availableDevices[i] << std::endl;
  }

  std::string iInputDevice;
  std::cin >> iInputDevice;
  // choose a device
  
  // set the device
  if (!setDevice(availableDevices[stoi(iInputDevice)]))
  {
    std::cout << "An error occured!" << std::endl;
  }

  setProcessingInterval(sf::milliseconds(5));
}

bool AudioRecorder::onStart() // optional
{
  std::cout << "Started recording :D" << std::endl;
  mAudioStream.start();
  return true;
}

bool AudioRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{




  // convert to chars
  char * samplesAsChars = new char[sampleCount*2]();
  samplesAsChars = reinterpret_cast<char*>(const_cast<sf::Int16*>(samples));


  // push to AudioStream for decoding and playing
  mAudioStream.feed(samplesAsChars, sampleCount);


//   sf::SoundBuffer mBuffer;
//   mBuffer.loadFromSamples(samples,sampleCount,1,44100);
// 
//   std::thread pushThread(&AudioRecorder::pushToBufferQueue, this, mBuffer);
//   pushThread.detach();
// 
//   if(!mLivePlaying) playLiveSound();
  return true;
}

// void AudioRecorder::pushToBufferQueue(sf::SoundBuffer iSoundBuffer)
// {
//     std::lock_guard<std::mutex> lock(mQueueMutex);
//     mSoundBufferQueue.push_back(iSoundBuffer);
// }

// void AudioRecorder::playLiveSound()
// {
//   while(mSoundBufferQueue.size() > 0)
//   {
//     mLivePlaying = true;
//     sf::SoundBuffer wTempBuffer;
// 
//     sf::Sound wSound;
//     {
//       std::lock_guard<std::mutex> lock(mQueueMutex);
//       wTempBuffer = mSoundBufferQueue.front();
//       mSoundBufferQueue.pop_front();
//     }
// 
//     wSound.setBuffer(wTempBuffer);
// 
//     wSound.play();
//     std::cout << "<";
//     while(wSound.getStatus() != sf::Sound::Status::Stopped)
//     {
//     }
// 
//   }
//   mLivePlaying = false;
// }



void AudioRecorder::onStop() // optional
{
  std::cout << "Recording stopped :(" << std::endl;
}