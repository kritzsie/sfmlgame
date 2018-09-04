#include "music.hpp"

#include "util.hpp"

#include <snes_spc/spc.h>

#include <SFML/Audio.hpp>

#include <cmath>
#include <vector>

namespace ke {
spc_err_t SPC::load_spc(const std::vector<char>& spc) {
  return spc_load_spc(snes_spc, spc.data(), spc.size());
}

spc_err_t SPC::play(std::vector<short>& buffer) {
  return spc_play(snes_spc, buffer.size(), buffer.data());
}

void SPC::reset() {
  spc_reset(snes_spc);
}

void SPC::soft_reset() {
  spc_soft_reset(snes_spc);
}

void SPC::set_tempo(float new_tempo) {
  spc_set_tempo(snes_spc, std::round(new_tempo * tempo_unit));
}

float SPC::get_tempo() {
  return tempo;
}

SPC::SPC() {
  snes_spc = spc_new();
}

SPC::~SPC() {
  if (snes_spc != nullptr) spc_delete(snes_spc);
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

SPCStream::SPCStream() : SPCStream(2048, 2) {}

SPCStream::~SPCStream() {
  if (getStatus() == sf::SoundSource::Status::Playing) {
    stop();
  }
}

bool Music::change(std::string name) {
  std::string path = "/music/" + name + ".spc";

  if (stream->spc.load_spc(util::readFile(path))) {
    return false;
  }

  return true;
}

void Music::play() {
  stream->play();
}

void Music::pause() {
  stream->pause();
}

void Music::resume() {
  if (stream->getStatus() == sf::SoundSource::Status::Paused) {
    stream->play();
  }
}

void Music::stop() {
  stream->stop();
}

Music::Music() {
  stream = new SPCStream;
}

Music::~Music() {
  if (stream != nullptr) delete stream;
}
}
