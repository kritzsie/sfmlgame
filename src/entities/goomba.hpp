#pragma once

#include "entity.hpp"

namespace ke {
class Goomba : public Entity {
private:
  Vec2f max_vel = Vec2f(192.f, 256.f);

  void resolveEntityCollisions();

  void resolveWorldCollisions();

public:
  enum class State {
    alive, dead
  };

  static Factory create();

  State state = State::alive;

  float timer = 0.f;

  void update() override;

protected:
  Goomba(Engine*, World*);
};
}
