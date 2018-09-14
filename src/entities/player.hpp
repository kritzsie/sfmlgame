#pragma once

#include "baseentity.hpp"
#include "entity.hpp"

namespace ke {
class Player : public Entity {
public:
  static Factory create();

  void update() override;

  ~Player() override = default;

protected:
  Player(Engine*, World*);
};
}
