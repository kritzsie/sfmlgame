#include "entity.hpp"
#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace entity {
// BaseEntity skipped; no methods
// RenderEntity
void RenderEntity::flipX() {
  scale.x *= -1;
}

void RenderEntity::flipY() {
  scale.y *= -1;
}

// Helper method to transform from world to screen coordinates
Vector2f RenderEntity::toView() {
  return Vector2f(pos.x - (offset.x * scale.x * facing), -(pos.y - (offset.y * scale.y) + texture.getSize().y));
}

RenderEntity::RenderEntity() : offset(0, 0), scale(1, 1), facing(1) {}

RenderEntity::RenderEntity(
  const Vector2f& offset,
  const Vector2f& scale
) : offset(offset), scale(scale), facing(1) {}

// Use Entity for things with collision
void Entity::applyForce(const Vector2f& f) {
  netForce += f;
}

void Entity::applyForce(float x, float y) {
  netForce += {x, y};
}

Entity::Entity() : RenderEntity({4, 0}, {1, 1}), mass(1), radius(8), height(16) {}

Entity::Entity(
  float mass,
  float radius,
  float height
) : RenderEntity({radius / 2, 0}, {1, 1}), mass(mass), radius(radius), height(height) {}

Entity::Entity(
  const Vector2f& offset,
  const Vector2f& scale,
  float mass = 1,
  float radius = 8,
  float height = 16
) : RenderEntity(offset, scale), mass(mass), radius(radius), height(height) {}

// Players inherit from Entity, but also hold states (jumping, crouching, etc.)
PlayerEntity::PlayerEntity() : Entity({7, 0}, {1, 1}) {}

PlayerEntity::PlayerEntity(
  float mass,
  float radius,
  float height
) : Entity(mass, radius, height) {}

PlayerEntity::PlayerEntity(
  const Vector2f& offset,
  const Vector2f& scale
) : Entity(offset, scale, 85, 5, 24) {}

PlayerEntity::PlayerEntity(
  const Vector2f& offset,
  const Vector2f& scale,
  float mass,
  float radius,
  float height
) : Entity(offset, scale, mass, radius, height) {}
}
