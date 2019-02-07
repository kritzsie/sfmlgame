#include "basegame.hpp"

#include "overworld.hpp"
#include "../engine.hpp"
#include "../util.hpp"

#include <cmath>

namespace ke {
BaseGame::Factory BaseGame::create() {
  return [](Engine* engine) -> BaseState* {
    return new BaseGame(engine);
  };
}

uint BaseGame::getCoins() const {
  return coins;
}

uint BaseGame::getLives() const {
  return lives;
}

ulong BaseGame::getScore() const {
  return score;
}

void BaseGame::addCoins(int count) {
  coins = util::absmod(coins + count, 100);
}

void BaseGame::addLives(int count) {
  lives = std::max(0, std::min(lives + count, 99));
}

void BaseGame::addScore(int count) {
  score += count;
}

void BaseGame::addScore(long count) {
  score += count;
}

void BaseGame::addScore(ulong count) {
  score += count;
}

void BaseGame::enter() {
  resume();
}

void BaseGame::exit() {}

void BaseGame::pause() {
  paused = true;
}

void BaseGame::resume() {
  paused = false;
}

void BaseGame::update() {
  if (not paused) {
    engine->pushState(Overworld::create(this));
    pause();
  }
}

void BaseGame::draw() {}

BaseGame::BaseGame(Engine* engine) : BaseState(engine) {}
}
