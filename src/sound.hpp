#pragma once

#include "assetmanager.hpp"

#include <SFML/Audio.hpp>

#include <map>
#include <string>

namespace ke {
class Sound {
public:
  static const std::size_t MAX_VOICES = 32;

private:
  SFXAssetManager& sfxassets;
  sf::Sound voices[MAX_VOICES];

public:
  bool play(std::size_t, std::string);
  std::size_t play(std::string);
  bool pause(std::size_t);
  std::size_t pause();
  bool resume(std::size_t);
  std::size_t resume();
  bool stop(std::size_t);
  std::size_t stop();

  Sound(SFXAssetManager&);
};
}
