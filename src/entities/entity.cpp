#include "entity.hpp"

namespace ke {
Rect<float> Entity::getBBox() const {
  return Rect<float>(Vec2f(pos.x - radius, pos.y), Vec2(radius * 2.f, height));
}

Entity::Entity(EntityType type, Engine* engine, World* world, float radius, float height)
: RenderEntity(type, engine, world), radius(radius), height(height) {}

Entity::Entity(EntityType type, Engine* engine, World* world)
: Entity(type, engine, world, 8.f, 16.f) {}
}
