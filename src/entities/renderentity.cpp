#include "renderentity.hpp"

#include "../engine.hpp"
#include "../types.hpp"

#include <cmath>
#include <cstddef>

namespace ke {
byte Sprite::getDirection() const {
  return direction;
}

void Sprite::setDirection(byte d) {
  direction = std::copysign(1, d);
}

Sprite::Sprite() : RenderStates(), scale(1.f) {}

RenderEntity::RenderEntity(EntityType type, Engine* engine, World* world)
: BaseEntity(type, engine, world), Sprite() {}
}
