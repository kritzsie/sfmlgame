#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

namespace entity {
using namespace types;

using geometry::rect::Rect;
using geometry::vector2::Vector2;

using Vector2f = Vector2<float>;
using Vector2u = Vector2<uint>;

using enttype_t = uint;

class BaseEntity {
public:
  enttype_t type;
  Vector2f pos;
  Vector2f vel;
};

class RenderEntity : public BaseEntity {
protected:
  byte direction;

public:
  sf::Sprite sprite;
  Vector2f offset;
  Vector2f scale;

  void flipX();
  void flipY();

  byte getDirection() const;
  void setDirection(byte);

  Vector2f toView();

  RenderEntity();
  RenderEntity(const Vector2f&, const Vector2f&);
};

class Entity : public RenderEntity {
public:
  float radius, height;

  Rect<float> getAABB() const;

  Entity();
  Entity(float, float);
  Entity(const Vector2f&, const Vector2f&, float, float);
};

class PlayerEntity : public Entity {
public:
  bool airborne = false;
  bool crouching = false, p_speed = false;
  bool underwater = false, on_ice = false;

  float jumptime = 0;

  PlayerEntity();
  PlayerEntity(float, float);
  PlayerEntity(const Vector2f&, const Vector2f&);
  PlayerEntity(const Vector2f&, const Vector2f&, float, float);
};
}

namespace keng {
  using namespace entity;
}

#endif // ENTITY_HPP
