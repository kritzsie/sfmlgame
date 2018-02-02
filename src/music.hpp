#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <snes_spc/spc.h>

#include <SFML/Audio.hpp>

#include <map>
#include <string>
#include <vector>

namespace music {
class SPC {
public:
  enum {
    sample_rate = spc_sample_rate,
    rom_size = spc_rom_size,
    clock_rate = spc_clock_rate,
    clocks_per_sample = spc_clocks_per_sample,
  };

private:
  SNES_SPC* snes_spc;

public:
  spc_err_t load_spc(std::vector<char>);
  spc_err_t play(std::vector<short>&);
  void reset();
  void soft_reset();

  SPC();
  ~SPC();
};

class SPCStream : public sf::SoundStream {
public:
  SPC spc;
  std::vector<short> buffer;

  virtual bool onGetData(Chunk&);
  virtual void onSeek(sf::Time);

  SPCStream(std::size_t, std::size_t);
};

class Music {
public:
  SPCStream stream;
  std::map<std::string, std::vector<char>> songs;

  bool init();
  bool change(const char*);
  void play();
  void pause();
  void resume();
  void stop();

  Music();
};
}

namespace keng {
  using namespace music;
}

#endif // MUSIC_HPP

