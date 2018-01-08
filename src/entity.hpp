#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "geometry.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace entity {
using uint_t = unsigned int;

using enttype_t = uint_t;

using keng::Vector2;
using Vector2f = Vector2<float>;
using Vector2u = Vector2<uint_t>;

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
  Vector2<Vector2f> aabb;

  void applyForce(const float, const float);
  void applyForce(const Vector2f&);

  Entity();
  Entity(const float, const float, const float);
  Entity(const Vector2f&, const Vector2f&, const float, const float, const float);
};

class PlayerEntity : public Entity {
public:
  struct State {
    bool jumping = true;
    bool crouching, p_speed;
    bool underwater, on_ice;
  } state {};

  PlayerEntity();
  PlayerEntity(const float, const float, const float);
  PlayerEntity(const Vector2f&, const Vector2f&);
  PlayerEntity(const Vector2f&, const Vector2f&, const float, const float, const float);
};
}

namespace keng {
  using namespace entity;
}

#endif
