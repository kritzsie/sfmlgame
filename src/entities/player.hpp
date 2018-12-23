#pragma once

#include <optional>

#include "entity.hpp"
#include "../input.hpp"
#include "../types.hpp"

namespace ke {
class Player : public Entity {
private:
  Vec2f max_vel = Vec2f(192.f, 256.f);

  void resolveEntityCollisionsX();
  void resolveEntityCollisionsY();

  void resolveWorldCollisionsX();
  void resolveWorldCollisionsY();

  void updateState();

public:
  // TODO: implement states as mutually exclusive fields (enum classes)
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

  int state = State::idle;

  float p_meter = 0.f;

  float state_timer = 0.f;
  float turn_timer = 0.f;
  float jump_timer = 0.f;
  float death_timer = 0.f;

  std::optional<std::size_t> snd_running;

  void jump(const Input&);
  void duck();
  void stand();
  void die();

  void update() override;

protected:
  Player(Engine*, World*);
};
}
