#pragma once

#include "baseentity.hpp"
#include "entity.hpp"

namespace ke {
class Player : public Entity {
private:
  void resolveEntityCollisions();
  void resolveWorldCollisions();

public:
  enum State : int {
    idle,
    airborne   = 1 << 0,
    ducking    = 1 << 1,
    p_speed    = 1 << 2,
    underwater = 1 << 3
  };

  static Factory create();

  int state = idle;

  float jumptime;
  float sliptime;

  Vec2f max_vel;

  void update() override;

  ~Player() override = default;

protected:
  Player(Engine*, World*);
};
}
