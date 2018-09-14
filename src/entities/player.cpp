#include "player.hpp"
#include "../engine.hpp"

#include <cmath>

namespace ke {
Player::Factory Player::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Player(engine, world);
  };
}

void Player::resolveEntityCollisions() {}

void Player::resolveWorldCollisions() {}

void Player::update() {
  if (vel.y < max_vel.y) {
    vel.y = std::min(
      -max_vel.y,
      vel.y + std::max(world->gravity / std::pow(engine->ticktime.rate, 2.f),
      max_vel.y)
    );
  }
  pos += vel / engine->ticktime.rate;
  resolveEntityCollisions();
  resolveWorldCollisions();
}

Player::Player(Engine* engine, World* world)
: Entity(engine, world, 5.f, 26.f), max_vel(0.f, 256.f) {
  pushFrame("stand", "bigmariowalk_0", Rect(0, 0, 14, 27), Vec2(7.f, 0.f), 0.f);
  setState("stand", 0);
}
}
