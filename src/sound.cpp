#include "sound.hpp"

#include <cstddef>

namespace sound {
bool Sound::init() {
  sounds.emplace("jump", sf::SoundBuffer());
  sounds.emplace("brickshatter", sf::SoundBuffer());

  for (auto& it : sounds) {
    it.second.loadFromFile(it.first + ".wav");
  }

  return true;
}

void Sound::play(const char* snd_id) {
  const auto& cur_snd = sounds.at(snd_id);

  for (size_t i = 0; i < MAX_VOICES - 1; i++) {
    if (voices[i].getBuffer() == &cur_snd) {
      voices[i].play();
      return;
    }
    else if (voices[i].getStatus() == sf::SoundSource::Status::Stopped) {
      voices[i].setBuffer(cur_snd);
      voices[i].play();
      return;
    }
  }

  if (voices[MAX_VOICES - 1].getBuffer() != &cur_snd) {
    voices[MAX_VOICES - 1].setBuffer(cur_snd);
  }
  voices[MAX_VOICES - 1].play();
}

void Sound::pause() {
  for (size_t i = 0; i < MAX_VOICES; i++) {
    voices[i].pause();
  }
}

void Sound::resume() {
  for (size_t i = 0; i < MAX_VOICES; i++) {
    if (voices[i].getStatus() == sf::SoundSource::Status::Paused) {
      voices[i].play();
    }
  }
}

void Sound::stop(const char* snd_id) {
  const auto& cur_snd = sounds.at(snd_id);

  for (size_t i = 0; i < MAX_VOICES; i++) {
    if (voices[i].getBuffer() == &cur_snd) {
      voices[i].stop();
      break;
    }
  }
}
}
