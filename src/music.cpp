#include "music.hpp"
#include "util.hpp"

#include <snes_spc/spc.h>

#include <SFML/Audio.hpp>

#include <vector>

namespace music {
spc_err_t SPC::load_spc(std::vector<char> spc) {
  return spc_load_spc(snes_spc, spc.data(), spc.size());
}

void SPC::reset() {
  spc_reset(snes_spc);
}

void SPC::soft_reset() {
  spc_soft_reset(snes_spc);
}

spc_err_t SPC::play(std::vector<short>& buffer) {
  return spc_play(snes_spc, buffer.size(), buffer.data());
}

SPC::SPC() {
  snes_spc = spc_new();
}

SPC::~SPC() {
  spc_delete(snes_spc);
}

bool SPCStream::onGetData(Chunk& data) {
  if (spc.play(buffer)) {
    return false;
  }
  data.samples = buffer.data();
  data.sampleCount = buffer.size();
  return true;
}

void SPCStream::onSeek(sf::Time) {}

SPCStream::SPCStream(std::size_t bufsize, std::size_t channels) : buffer(bufsize) {
  initialize(channels, 32000);
}

bool Music::init() {
  songs.emplace("overworld", std::vector<char>());
  songs.emplace("playerdown", std::vector<char>());

  for (auto& it : songs) {
    it.second = util::load_file("assets/music/" + it.first + ".spc");
  }

  return true;
}

bool Music::change(const char* song_id) {
  if (stream.spc.load_spc(songs.at(song_id))) {
    return false;
  }

  return true;
}

void Music::play() {
  stream.play();
}

void Music::pause() {
  stream.pause();
}

void Music::resume() {
  if (stream.getStatus() == sf::SoundSource::Status::Paused) {
    stream.play();
  }
}

void Music::stop() {
  stream.stop();
}

Music::Music() : stream(2048, 2) {}
}
