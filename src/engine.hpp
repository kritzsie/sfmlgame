#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "geometry.hpp"
#include "music.hpp"
#include "sound.hpp"
#include "types.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <map>
#include <string>
#include <vector>

#include <cstddef>
#include <cstdint>

namespace engine {
using types::byte;
using types::uint;

using geometry::vec2::Vec2;
using Vec2f = Vec2<float>;

using music::Music;
using sound::Sound;
using world::World;

using arglist = std::vector<std::string>;

class Engine {
protected:
  const arglist& args;
  uint tick;
  sf::Clock tickClock;
  float tickTime, tickRate;
  sf::Event event;
  sf::RenderWindow* window;
  std::map<std::string, sf::Texture> backgrounds, sprites, tiles;
  Music* music;
  Sound* sound;
  World* world;

  struct Keys {
    class State {
    private:
      bool state = false;
      byte delta = 0;

    public:
      bool getState() const;
      byte getDelta() const;

      void setState(bool);
      void press();
      void release();
      void resetDelta();
    };

    State up, left, down, right;
    State jump, run;
  } keys;

  bool init();
  void resize(size_t, size_t);
  void onKeyEvent();
  void tickKeys();
  void doTick();
  void drawBG(uint32_t);
  void drawBG(const char*, float, float);
  void drawBGBottom(const char*, float, float);
  void drawBGTop(const char*, float, float);
  void drawTiles();
  void drawEntities();
  void doRender();

public:
  int exec();
  Engine(const arglist&);
  ~Engine();
};
}

namespace keng {
  using namespace engine;
}

#endif // ENGINE_HPP
