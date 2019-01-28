
#include "AudioAccumulator.h"
#include "AudioStream.h"

AudioAccumulator::AudioAccumulator()
{
  mRunning.store(true);

  mOutputThread = std::thread([&]()
  {
    AudioStream wAudioStream;
    std::vector<short> wAccumulatedSamples;
    CircularBuffer mAveragePacketSize(10);

    
    while (mRunning.load())
    {
      wAccumulatedSamples.clear();

      while (!empty())
      {
        std::lock_guard<std::mutex> lock(mMapMutex);
        wAccumulatedSamples.push_back(0);

        for (auto wMapElement : mBuffersMap)
        {
          wAccumulatedSamples.back() =+ wMapElement.second->pop();
        }
      }

      if (wAccumulatedSamples.size() > 0)
      {
        wAudioStream.feed(&wAccumulatedSamples[0], wAccumulatedSamples.size());
        mAveragePacketSize.push(wAccumulatedSamples.size());
        std::cout << "\r" << mAveragePacketSize.avg();
      }
      


      //std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }

    wAudioStream.stop();

  });

}


std::shared_ptr<CircularBuffer> AudioAccumulator::operator[](std::string& iSelectedBuffer)
{
  std::lock_guard<std::mutex> lock(mMapMutex);
  if (mBuffersMap.find(iSelectedBuffer) == mBuffersMap.end())
  {
    mBuffersMap[iSelectedBuffer] = std::make_shared<CircularBuffer>();
  }

  return mBuffersMap[iSelectedBuffer];
}

AudioAccumulator::~AudioAccumulator()
{
  mRunning.store(false);
  mOutputThread.join();
}

bool AudioAccumulator::empty()
{
  if (mBuffersMap.empty()) { return true; }

  std::lock_guard<std::mutex> lock(mMapMutex);
  for (auto wMapElement : mBuffersMap)
  {
    if (!wMapElement.second->empty())
    {
      return false;
    }
  }
  return true;
}