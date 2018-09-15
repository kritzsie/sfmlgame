#include "entity.hpp"

namespace ke {
Entity::Factory Entity::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Entity(engine, world);
  };
}

Rect<float> Entity::getBBox() const {
  return Rect<float>(Vec2f(pos.x - radius, pos.y), Vec2(radius * 2.f, height));
}

Entity::Entity(Engine* engine, World* world, float radius, float height)
: RenderEntity(engine, world), radius(radius), height(height) {}

Entity::Entity(Engine* engine, World* world)
: Entity(engine, world, 8.f, 16.f) {}
}
