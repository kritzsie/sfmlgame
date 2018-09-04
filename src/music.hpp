#pragma once

#include <snes_spc/spc.h>

#include <SFML/Audio.hpp>

#include <map>
#include <string>
#include <vector>

namespace ke {
class SPC {
public:
  enum {
    sample_rate = spc_sample_rate,
    rom_size = spc_rom_size,
    clock_rate = spc_clock_rate,
    clocks_per_sample = spc_clocks_per_sample,
    tempo_unit = spc_tempo_unit
  };

private:
  SNES_SPC* snes_spc = nullptr;
  float tempo = 0.0f;

public:
  spc_err_t load_spc(const std::vector<char>&);
  spc_err_t play(std::vector<short>&);
  void reset();
  void soft_reset();
  void set_tempo(float);
  float get_tempo();

  SPC();
  ~SPC();
};

class SPCStream : public sf::SoundStream {
private:
  std::vector<short> buffer;

public:
  SPC spc;

  bool onGetData(Chunk&);
  void onSeek(sf::Time);

  SPCStream();
  SPCStream(std::size_t, std::size_t);
  ~SPCStream();
};

class Music {
private:
  SPCStream* stream = nullptr;

public:
  bool change(std::string);
  void play();
  void pause();
  void resume();
  void stop();

  Music();
  ~Music();
};
}
