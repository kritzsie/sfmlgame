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

using geometry::vector2::Vector2;
using Vector2f = Vector2<float>;
using Vector2u = Vector2<uint>;

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
      bool my_state = false;
      byte my_delta = 0;

    public:
      bool state() const;
      byte delta() const;

      void set(bool);
      void press();
      void release();
      void tick();
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
