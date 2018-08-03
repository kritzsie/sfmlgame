#pragma once

#include <SFML/Audio.hpp>

#include <map>
#include <string>

namespace ke {
class Sound {
public:
  static const size_t MAX_VOICES = 32;

  sf::Sound voices[MAX_VOICES];
  std::map<std::string, sf::SoundBuffer> sounds;

  bool init();
  void play(const char*);
  void pause();
  void resume();
  void stop(const char*);
};
}
