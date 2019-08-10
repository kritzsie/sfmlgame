#pragma once

#include "entityinfo.hpp"

#include "../geometry.hpp"
#include "../types.hpp"

#include <cstdint>
#include <functional>

namespace ke {
class Engine;
class World;

class BaseEntity {
public:
  const EntityType type;

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
  BaseEntity(EntityType, Engine*, World*);

private:
  BaseEntity(Engine*, World*);
};
}
