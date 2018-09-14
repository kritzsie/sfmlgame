#include "player.hpp"

namespace ke {
Player::Factory Player::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Player(engine, world);
  };
}

void Player::update() {
}

Player::Player(Engine* engine, World* world) : Entity(engine, world) {}
}
