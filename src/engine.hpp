#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "geometry.hpp"
#include "types.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <map>
#include <string>
#include <vector>

#include <cstddef>

namespace engine {
using namespace types;
using arglist = std::vector<char*>;

using geometry::vec2::Vec2;
using Vec2f = Vec2<float>;

using world::World;

class Engine {
protected:
  const arglist& args;
  uint tick;
  sf::Clock tickClock;
  float tickTime, tickRate;
  sf::Event event;
  sf::RenderWindow* window;
  std::map<std::string, sf::Texture> textures;
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
  void drawBG(const sf::Texture&, float, float);
  void drawBGBottom(const sf::Texture&, float, float);
  void drawBGTop(const sf::Texture&, float, float);
  void drawTiles();
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
