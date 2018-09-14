#include "entity.hpp"

namespace ke {
Entity::Factory Entity::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Entity(engine, world);
  };
}

Vec2f Entity::getSize() const {
  return Vec2(radius * 2.f, height);
}

Rect<float> Entity::getBBox() const {
  return Rect<float>(pos, pos + getSize());
}

Entity::Entity(Engine* engine, World* world, float radius, float height)
: RenderEntity(engine, world), radius(radius), height(height) {}

Entity::Entity(Engine* engine, World* world) : Entity(engine, world, 8.f, 16.f) {}
}
