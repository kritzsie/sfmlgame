#include "baseentity.hpp"
#include "../engine.hpp"

namespace ke {
BaseEntity::Factory BaseEntity::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new BaseEntity(engine, world);
  };
}

EntityType BaseEntity::getType() const {
  return type;
}

void BaseEntity::update() {
  pos += vel / engine->ticktime.rate;
}

BaseEntity::BaseEntity(EntityType type, Engine* engine, World* world)
: type(type), engine(engine), world(world) {}

BaseEntity::BaseEntity(Engine* engine, World* world)
: BaseEntity(EntityType::Marker, engine, world) {}
}
