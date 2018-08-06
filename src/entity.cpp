#include "entity.hpp"

#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <string>
#include <utility>
#include <vector>

#include <ctgmath>

namespace ke {
// BaseEntity skipped; no methods
// RenderEntity
void RenderEntity::flipX() {
  scale.x *= -1;
}

void RenderEntity::flipY() {
  scale.y *= -1;
}

byte RenderEntity::getDirection() const {
  return direction;
}

void RenderEntity::setDirection(byte d) {
  direction = std::copysign(1, d);
}

// Helper method to transform from world to screen coordinates
Vec2f RenderEntity::toView() const {
  return Vec2f(
    pos.x - (offset.x * scale.x * direction),
    -(pos.y + (offset.y * scale.y) + sprite.getTexture()->getSize().y * scale.y)
  );
}

RenderEntity::RenderEntity() : direction(1), offset(0, 0), scale(1, 1) {}

RenderEntity::RenderEntity(
  const Vec2f& offset,
  const Vec2f& scale
) : direction(1) , offset(offset), scale(scale) {}

// Use Entity for things with collision
Rect<float> Entity::getAABB() const {
  return Rect<float>(pos.x - radius, pos.y, radius * 2, height);
}

Entity::Entity() : RenderEntity(Vec2f(8, 0), Vec2f(1, 1)), radius(8), height(16) {}

Entity::Entity(
  float radius,
  float height
) : RenderEntity(Vec2f(radius / 2, 0), Vec2f(1, 1)), radius(radius), height(height) {}

Entity::Entity(
  const Vec2f& offset,
  const Vec2f& scale,
  float radius = 8,
  float height = 16
) : RenderEntity(offset, scale), radius(radius), height(height) {}

RenderState::RenderState(std::string sprite, Vec2f offset) :
  sprite(sprite), offset(offset)
{}

// Players inherit from Entity, but also hold states (jumping, crouching, etc.)
PlayerEntity::PlayerEntity() : Entity() {
  std::vector<RenderState> walk_states;
  walk_states.push_back(RenderState("mariobigwalk_0", Vec2f(7, 0)));
  walk_states.push_back(RenderState("mariobigwalk_1", Vec2f(9, 0)));
  walk_states.push_back(RenderState("mariobigwalk_2", Vec2f(9, 0)));
  states.emplace("walk", std::move(walk_states));

  std::vector<RenderState> jump_states;
  jump_states.push_back(RenderState("mariobigjump", Vec2f(8, 0)));
  states.emplace("jump", std::move(walk_states));

  std::vector<RenderState> duck_states;
  duck_states.push_back(RenderState("mariobigduck", Vec2f(7, 0)));
  states.emplace("duck", std::move(walk_states));
}

void PlayerEntity::stand() {
  ducking = false;
  height = 25;
}

void PlayerEntity::jump() {
  vel.y = 176 + std::abs(vel.x / 16);
}

void PlayerEntity::duck() {
  ducking = true;
  height = 15;
}

PlayerEntity::PlayerEntity(
  float radius,
  float height
) : Entity(radius, height) {}

PlayerEntity::PlayerEntity(
  const Vec2f& offset,
  const Vec2f& scale
) : Entity(offset, scale, 5, 25) {}

PlayerEntity::PlayerEntity(
  const Vec2f& offset,
  const Vec2f& scale,
  float radius,
  float height
) : Entity(offset, scale, radius, height) {}
}
