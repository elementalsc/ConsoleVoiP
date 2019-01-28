#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(unsigned int iBufferSize) :
  mDataPtr(new short[iBufferSize]()),
  mBufferSize(iBufferSize),
  mHead(0),
  mTail(0)
{
  for (unsigned int i = 0; i < iBufferSize; ++i)
  {
    mDataPtr[i] = 0;
  }
}

CircularBuffer::~CircularBuffer()
{
  delete[] mDataPtr;
}


void CircularBuffer::push(short iData)
{
  std::lock_guard<std::mutex> lock(mBufferLock);
  mDataPtr[mHead] = iData;
  mHead = (mHead + 1) % mBufferSize;

  if (mHead == mTail)
  {
    mTail = (mTail + 1) % mBufferSize;
  }
}

void CircularBuffer::push(short* iData, size_t iDataLength)
{
  for (unsigned int i = 0; i < iDataLength; ++i)
  {
    push(iData[i]);
  }
}


short CircularBuffer::pop()
{
  std::lock_guard<std::mutex> lock(mBufferLock);
  short wReturnedValue = mDataPtr[mTail];
  mTail = (mTail + 1) % mBufferSize;
  return wReturnedValue;
}

bool CircularBuffer::empty()
{
  std::lock_guard<std::mutex> lock(mBufferLock);
  return mTail == mHead;
}


double CircularBuffer::avg()
{
  std::lock_guard<std::mutex> lock(mBufferLock);

  long sum = 0;
  long count = 0;
  unsigned int i = mTail;

  while (i != mHead)
  {
    sum += mDataPtr[i];
    i = (i + 1) % mBufferSize;
    ++count;
  }

  return sum / count;
}

