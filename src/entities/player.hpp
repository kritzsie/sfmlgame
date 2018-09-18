#pragma once

#include "baseentity.hpp"
#include "entity.hpp"
#include "../input.hpp"

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
    slipping   = 1 << 1,
    ducking    = 1 << 2,
    airborne   = 1 << 3,
    underwater = 1 << 4,
    running    = 1 << 5
  };

  static Factory create();

  int state = State::idle;

  float walktime;
  float sliptime;
  float jumptime;

  void jump(const Input&);
  void duck();
  void stand();

  void update() override;

protected:
  Player(Engine*, World*);
};
}
