#pragma once

#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

#include <map>
#include <string>

#include <cstddef>

namespace ke {
using enttype_t = uint;

class BaseEntity {
public:
  enttype_t type;
  Vec2f pos;
  Vec2f vel;
};

class RenderEntity : public BaseEntity {
protected:
  byte direction;

public:
  sf::Sprite sprite;
  Vec2f offset;
  Vec2f scale;

  void flipX();
  void flipY();

  byte getDirection() const;
  void setDirection(byte);

  Vec2f toView() const;

  RenderEntity();
  RenderEntity(const Vec2f&, const Vec2f&);
};

class Entity : public RenderEntity {
public:
  float radius, height;

  Rect<float> getAABB() const;

  Entity();
  Entity(float, float);
  Entity(const Vec2f&, const Vec2f&, float, float);
};

class RenderState {
public:
  std::string sprite;
  Vec2f offset;

  RenderState(std::string, Vec2f);
};

class PlayerEntity : public Entity {
public:
  std::size_t state_counter = 0;
  StringTable<std::vector<RenderState>> states;

  bool airborne = false;
  bool ducking = false, p_speed = false;
  bool underwater = false, on_ice = false;

  float jumptime = 0;
  float sliptime = 0;
  float walktime = 0;
  byte walkcycle = 0;

  void stand();
  void jump();
  void duck();

  PlayerEntity();
  PlayerEntity(float, float);
  PlayerEntity(const Vec2f&, const Vec2f&);
  PlayerEntity(const Vec2f&, const Vec2f&, float, float);
};
}
