#include "basegame.hpp"

#include "overworld.hpp"
#include "../engine.hpp"

namespace ke {
BaseGame::Factory BaseGame::create() {
  return [](Engine* engine) -> BaseState* {
    return new BaseGame(engine);
  };
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
