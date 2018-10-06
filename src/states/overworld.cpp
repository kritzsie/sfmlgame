#include "overworld.hpp"

#include "basegame.hpp"
#include "gameplay.hpp"
#include "../engine.hpp"

namespace ke {
Overworld::Factory Overworld::create(BaseGame* previous) {
  return [=](Engine* engine) -> BaseState* {
    return new Overworld(engine, previous);
  };
}

void Overworld::enter() {
  resume();
}

void Overworld::exit() {}

void Overworld::pause() {
  state = State::paused;
}

void Overworld::resume() {
  state = State::idle;
}

void Overworld::update() {
  if (state != State::paused) {
    engine->pushState(Gameplay::create(basegame));
    pause();
  }
}

void Overworld::draw() {}

Overworld::Overworld(Engine* engine, BaseGame* previous)
: BaseState(engine), basegame(previous) {}
}
