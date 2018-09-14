#include "renderentity.hpp"

namespace ke {
RenderEntity::Factory RenderEntity::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new RenderEntity(engine, world);
  };
}

std::string RenderEntity::getTexture() const {
  return "";
}

RenderEntity::RenderEntity(Engine* engine, World* world) : BaseEntity(engine, world) {}
}
