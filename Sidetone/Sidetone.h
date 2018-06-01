#include <iostream>
#include <SFML/Audio.hpp>
#include "../Audio/AudioStream.h"

class Sidetone : public sf::SoundRecorder
{
public:
  Sidetone();
  ~Sidetone();
  void enable(const bool& iEnable);
private:

  bool onStart();
  void onStop();
  bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
  AudioStream mAudioStream;
  bool mEnable;

};