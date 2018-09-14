#include "baseentity.hpp"
#include "../engine.hpp"

namespace ke {
BaseEntity::Factory BaseEntity::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new BaseEntity(engine, world);
  };
}

void BaseEntity::update() {
  pos += vel / engine->ticktime.rate;
}

BaseEntity::BaseEntity(Engine* engine, World* world)
: engine(engine), world(world) {}
}
