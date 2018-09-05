#include "intro.hpp"

#include "gameplay.hpp"

#include <cstdlib>

namespace ke {
GameState::Factory Intro::makeState() {
  return [](Engine* engine) -> GameState* {
    return new Intro(engine);
  };
}

void Intro::enter() {}
void Intro::exit() {}

void Intro::pause() {}
void Intro::resume() {}

void Intro::update() {
  engine->pop_state();
  engine->push_state(Gameplay::makeState());
}

void Intro::draw() {}

Intro::Intro(Engine* engine) : GameState(engine) {}
}
