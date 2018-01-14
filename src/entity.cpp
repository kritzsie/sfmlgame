#include "entity.hpp"
#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <ctgmath>

namespace entity {
// BaseEntity skipped; no methods
// RenderEntity
void RenderEntity::flipX() {
  scale.x *= -1;
}

void RenderEntity::flipY() {
  scale.y *= -1;
}

signed char RenderEntity::getDirection() {
  return direction;
}

void RenderEntity::setDirection(signed char d) {
  direction = std::copysign(1, d);
}

// Helper method to transform from world to screen coordinates
Vector2f RenderEntity::toView() {
  return Vector2f(
    pos.x - (offset.x * scale.x * direction),
    -(pos.y - (offset.y * scale.y) + texture.getSize().y * scale.y)
  );
}

RenderEntity::RenderEntity() : offset(0, 0), scale(1, 1), direction(1) {}

RenderEntity::RenderEntity(
  const Vector2f& offset,
  const Vector2f& scale
) : offset(offset), scale(scale), direction(1) {}

// Use Entity for things with collision
Rect<float> Entity::getAABB() const {
  return Rect<float>(pos.x - radius, pos.y, radius * 2, height);
}

Entity::Entity() : RenderEntity({8, 0}, {1, 1}), radius(8), height(16) {}

Entity::Entity(
  float radius,
  float height
) : RenderEntity({radius / 2, 0}, {1, 1}), radius(radius), height(height) {}

Entity::Entity(
  const Vector2f& offset,
  const Vector2f& scale,
  float radius = 8,
  float height = 16
) : RenderEntity(offset, scale), radius(radius), height(height) {}

// Players inherit from Entity, but also hold states (jumping, crouching, etc.)
PlayerEntity::PlayerEntity() : Entity() {}

PlayerEntity::PlayerEntity(
  float radius,
  float height
) : Entity(radius, height) {}

PlayerEntity::PlayerEntity(
  const Vector2f& offset,
  const Vector2f& scale
) : Entity(offset, scale, 5, 24) {}

PlayerEntity::PlayerEntity(
  const Vector2f& offset,
  const Vector2f& scale,
  float radius,
  float height
) : Entity(offset, scale, radius, height) {}
}
