#pragma once

#include "entity.hpp"

namespace ke {
class Goomba : public Entity {
private:
  Vec2f max_vel = Vec2f(192.f, 256.f);

  float timer = 0.f;

  bool valid = true;

  bool alive = true;
  bool airborne = true;

  void die();
  void think();
  void applyGravity();
  void resolveEntityCollisions();
  void resolveWorldCollisions();

public:
  static Factory create();

  bool isValid() const;

  bool isAlive() const;
  bool isAirborne() const;

  void update() override;

protected:
  Goomba(Engine*, World*);
};
}
