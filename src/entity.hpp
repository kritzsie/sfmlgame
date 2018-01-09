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
public:
  sf::Sprite sprite;
  sf::Texture texture;
  Vector2f offset;
  Vector2f scale;
  char facing;

  void flipX();
  void flipY();
  Vector2f toView();

  RenderEntity();
  RenderEntity(const Vector2f&, const Vector2f&);
};

class Entity : public RenderEntity {
public:
  float mass;
  Vector2f netForce;
  float radius, height;
  Rect<float> aabb;

  void applyForce(float, float);
  void applyForce(const Vector2f&);

  Entity();
  Entity(float, float, float);
  Entity(const Vector2f&, const Vector2f&, float, float, float);
};

class PlayerEntity : public Entity {
public:
  bool airborne;
  bool crouching, p_speed;
  bool underwater, on_ice;

  PlayerEntity();
  PlayerEntity(float, float, float);
  PlayerEntity(const Vector2f&, const Vector2f&);
  PlayerEntity(const Vector2f&, const Vector2f&, float, float, float);
};
}

namespace keng {
  using namespace entity;
}

#endif // ENTITY_HPP
