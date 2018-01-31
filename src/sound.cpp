#include "sound.hpp"

#include <cstddef>

namespace sound {
bool Sound::init() {
  sounds.emplace("jump", sf::SoundBuffer());

  for (auto& it : sounds) {
    it.second.loadFromFile(it.first + ".wav");
  }

  return true;
}

void Sound::play(const char* snd_id) {
  for (size_t i = 0; i < MAX_VOICES - 1; i++) {
    if (voices[i].getBuffer() == &sounds.at(snd_id)) {
      voices[i].play();
      return;
    }
    else if (voices[i].getStatus() == sf::SoundSource::Status::Stopped) {
      voices[i].setBuffer(sounds.at(snd_id));
      voices[i].play();
      return;
    }
  }
}
}
