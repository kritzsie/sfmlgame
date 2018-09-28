#pragma once

#include "baseentity.hpp"
#include "entity.hpp"
#include "../input.hpp"
#include "../types.hpp"

namespace ke {
class Player : public Entity {
private:
  Vec2f max_vel;

  void resolveEntityCollisionsX();
  void resolveEntityCollisionsY();

  void resolveWorldCollisionsX();
  void resolveWorldCollisionsY();

  void updateState();

public:
  enum State : int {
    idle       = 0,
    walking    = 1 << 0,
    turning    = 1 << 1,
    ducking    = 1 << 2,
    airborne   = 1 << 3,
    underwater = 1 << 4,
    running    = 1 << 5,
    slippery   = 1 << 6,
    dead       = 1 << 7
  };

  static Factory create();

  int state;

  float p_meter;

  float state_timer;
  float turn_timer;
  float jump_timer;
  float run_timer;
  float death_timer;

  void jump(const Input&);
  void duck();
  void stand();
  void die();

  void update() override;

protected:
  Player(Engine*, World*);
};
}
