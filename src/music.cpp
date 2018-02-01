#include "music.hpp"

#include <SFML/Audio.hpp>

namespace music {
void Music::init() {
  music.setLoop(true);
}

bool Music::change(const char* song_id) {
  if (music.openFromFile("assets/music/" + std::string(song_id) + ".ogg")) {
    play();
    return true;
  }

  return false;
}

void Music::play() {
  music.play();
}

void Music::pause() {
  music.pause();
}

void Music::resume() {
  if (music.getStatus() == sf::SoundSource::Status::Paused) {
    music.play();
  }
}

void Music::stop() {
  music.stop();
}
}
