#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "entity.hpp"
#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>

#include <cstddef>

namespace engine {
using namespace entity;
using namespace types;

using arglist = std::vector<char*>;
using tileid_t = uint;

using geometry::vector2::Vector2;
using Vector2f = Vector2<float>;
using Vector2u = Vector2<uint>;

class World {
protected:
  class State {
  public:
    BaseEntity camera;
    PlayerEntity player;
  };

public:
  tileid_t* tiles;
  Vector2<size_t> size;
  State prevState;
  State state;

  template<typename T>
  static T toView(const T&);
  tileid_t& getTile(const int, const int);
  void setTile(const int, const int, const tileid_t);
  bool init();

  World(const size_t, const size_t);
  ~World();
};

class Engine {
protected:
  const arglist& args;
  uint tick;
  sf::Clock tickClock;
  float tickTime, tickRate;
  sf::Event event;
  sf::RenderWindow* window;
  sf::Texture background;
  sf::Texture tileart;
  World* world;
  struct Keys {
    bool up, left, down, right;
    bool jump, run;
  } keys {};

  bool init();
  void updateKeys();
  void resize(const size_t, const size_t);
  void onTick();
  void render();

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
