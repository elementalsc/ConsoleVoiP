#include "Sidetone.h"


Sidetone::Sidetone() : 
  mEnable(false)
{
  
  // get the available sound input device names
  std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
  // 
  //   std::cout << "Select input device..." << std::endl;
  // 
  //   for(unsigned int i = 0; i < availableDevices.size(); ++i)
  //   {
  //     std::cout << i << " : " << availableDevices[i] << std::endl;
  //   }
  // 
  //   std::string iInputDevice;
  //    std::cin >> iInputDevice;
  //   // choose a device
  // 
  //   // set the device
  //   if (!setDevice(availableDevices[stoi(iInputDevice)]))
  if (!setDevice(availableDevices[0]))
  {
    std::cout << "An error occured!" << std::endl;
  }
  else
  {
    std::cout << "Capture device selected : " << availableDevices[0] << std::endl;
    setProcessingInterval(sf::milliseconds(20));
  }

  start();
  mAudioStream.start();
  enable(true);
}

Sidetone::~Sidetone()
{

}

void Sidetone::enable(const bool& iEnable)
{
  mEnable = iEnable;
  if(mEnable)
  {
    std::cout << "Enabled sidetone" << std::endl;
  }
  else
  {
    std::cout << "Disabled sidetone" << std::endl;
  }
}


bool Sidetone::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{

  if(mEnable)
  {
    mAudioStream.feed(samples,sampleCount);
  }

  // Continue processing
  return true;
}



bool Sidetone::onStart() // optional
{
  std::cout << "Started capture" << std::endl;
  return true;
}

void Sidetone::onStop() // optional
{
  std::cout << "Stopped Sidetone" << std::endl;
}