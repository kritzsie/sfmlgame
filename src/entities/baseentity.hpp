#pragma once

#include "../geometry.hpp"
#include "../types.hpp"

namespace ke {
class Engine;
class World;

class BaseEntity {
protected:
  Engine* const engine = nullptr;
  World* const world = nullptr;

public:
  using Factory = BaseEntity* (*)(Engine*, World*);

  static Factory create();

  Vec2f pos;
  Vec2f vel;

  virtual void update();

  virtual ~BaseEntity() = default;

protected:
  BaseEntity(Engine*, World*);
};
}
