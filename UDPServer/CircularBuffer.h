#pragma once

#define DEFAULT_BUFFER_SIZE 4096

#include <mutex>

struct CircularBuffer
{
  CircularBuffer(unsigned int iBufferSize = DEFAULT_BUFFER_SIZE);

  ~CircularBuffer();

  void push(short* iData, size_t iDataLength);
  void push(short iData);
  short pop();
  bool empty();
  double avg();

  size_t size() { return mBufferSize; }

private:
  unsigned int mHead;
  unsigned int mTail;
  short* mDataPtr;
  std::mutex mBufferLock;
  const unsigned int mBufferSize;
};

