#include "renderentity.hpp"

#include <cmath>

namespace ke {
RenderEntity::Factory RenderEntity::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new RenderEntity(engine, world);
  };
}

byte RenderEntity::getDirection() const {
  return direction;
}

void RenderEntity::setDirection(byte d) {
  direction = std::copysign(1, d);
}

std::string RenderEntity::getTexture() const {
  return "";
}

RenderEntity::RenderEntity(Engine* engine, World* world) : BaseEntity(engine, world) {}
}
