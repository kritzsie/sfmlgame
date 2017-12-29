#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "entity.hpp"
#include "geometry.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>

#include <cstddef>

namespace engine {
using namespace entity;
using namespace geometry;

using uint_t = unsigned int;

using arglist = std::vector<const char*>;
using tileid_t = uint_t;

using Vector2f = Vector2<float>;
using Vector2u = Vector2<uint_t>;

class World {
public:
  tileid_t* tiles;
  Vector2<size_t> size;
  PlayerEntity player;
  BaseEntity camera;

  template<typename T>
  static T toView(const T&);
  bool init();
  tileid_t& getTile(const int, const int);
  void setTile(const int, const int, const tileid_t);

  World(const size_t, const size_t);
  ~World();
};

class Engine {
private:
  const arglist& args;
  uint_t tick;
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
  } keys;

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

#endif
