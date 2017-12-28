#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "geometry.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>

#include <cstddef>

namespace engine {
using geometry::Vector2;
using Vector2f = Vector2<float>;
using Vector2s = Vector2<size_t>;
using Vector2u = Vector2<unsigned int>;

using arglist = std::vector<const char*>;

using uint_t = unsigned int;
using enttype_t = uint_t;

class BaseEntity {
public:
  enttype_t type;
  Vector2f pos;
  Vector2f vel;
};

class RenderEntity : public BaseEntity {
public:
  sf::Sprite sprite;
  sf::Texture texture;
  Vector2f offset;
  Vector2f scale;
  char facing;

  void flipX();
  void flipY();
  Vector2f toView();

  RenderEntity(const Vector2f&, const Vector2f&);
};

class Entity : public RenderEntity {
public:
  float mass;
  Vector2f netForce;

  void applyForce(const float, const float);
  void applyForce(const Vector2f&);

  Entity(const float, const Vector2f&, const Vector2f&);
};

class PlayerEntity : public Entity {
public:
  struct State {
    bool jumping, crouching, p_speed,
          underwater, on_ice;
  } state;

  PlayerEntity(const float, const Vector2f&, const Vector2f&);
};

using tileid_t = unsigned int;

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
