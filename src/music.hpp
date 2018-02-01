#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <SFML/Audio.hpp>

#include <map>
#include <string>

namespace music {
class Music {
public:
  sf::Music music;
  std::map<std::string, const char*> songs;

  void init();
  bool change(const char*);
  void play();
  void pause();
  void resume();
  void stop();
};
}

namespace keng {
  using namespace music;
}

#endif // MUSIC_HPP

