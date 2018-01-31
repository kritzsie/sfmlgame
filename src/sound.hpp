#ifndef SOUND_HPP
#define SOUND_HPP

#include <SFML/Audio.hpp>

#include <map>
#include <string>

namespace sound {
const size_t MAX_VOICES = 32;

class Sound {
public:
  sf::Sound voices[MAX_VOICES];
  std::map<std::string, sf::SoundBuffer> sounds;

  bool init();
  void play(const char*);
};
}

namespace keng {
  using namespace sound;
}

#endif // SOUND_HPP
