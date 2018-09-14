#include "entity.hpp"

namespace ke {
Entity::Factory Entity::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Entity(engine, world);
  };
}

Rect<float> Entity::getBBox() const {
  return Rect<float>();
}

Entity::Entity(Engine* engine, World* world) : RenderEntity(engine, world) {}
}
