#pragma once

#include <unordered_map>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include "CircularBuffer.h"

class AudioStream;

class AudioAccumulator
{
public:

  AudioAccumulator();
  ~AudioAccumulator();

  std::shared_ptr<CircularBuffer> operator[](std::string& iSelectedBuffer);
  bool empty();

private:
  std::unordered_map<std::string, std::shared_ptr<CircularBuffer>> mBuffersMap;
  std::thread mOutputThread;
  std::atomic_bool mRunning;
  AudioStream* mAudioStream;
  std::mutex mMapMutex;
};