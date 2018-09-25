#pragma once

#include "../geometry.hpp"
#include "../types.hpp"

#include <functional>

namespace ke {
class Engine;
class World;

class BaseEntity {
protected:
  Engine* const engine = nullptr;
  World* const world = nullptr;

public:
  using Factory = std::function<BaseEntity* (Engine*, World*)>;

  static Factory create();

  Vec2f pos;
  Vec2f vel;

  virtual void update();

  virtual ~BaseEntity() = default;

protected:
  BaseEntity(Engine*, World*);
};
}
