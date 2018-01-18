#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "entity.hpp"
#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <map>
#include <string>
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
  tileid_t* tiles;

public:
  Vector2<size_t> size;

  BaseEntity camera;
  PlayerEntity player;

  template<typename T>
  static T toView(const T&);
  template<typename T>
  static Rect<int> tilesFromAABB(const Rect<T>&);
  static Rect<float> tileAABB(int, int);
  tileid_t& getTile(int, int);
  void setTile(int, int, tileid_t);
  bool init();

  World(size_t, size_t);
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
