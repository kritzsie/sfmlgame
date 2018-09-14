#pragma once

#include "baseentity.hpp"
#include "entity.hpp"

namespace ke {
class Player : public Entity {
private:
  void resolveEntityCollisions();
  void resolveWorldCollisions();

public:
  static Factory create();

  Vec2f max_vel;

  void update() override;

  ~Player() override = default;

protected:
  Player(Engine*, World*);
};
}
