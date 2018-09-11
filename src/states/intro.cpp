#include "intro.hpp"

#include "gameplay.hpp"

namespace ke {
Intro::Factory Intro::makeState() {
  return [](Engine& engine) -> GameState* {
    return new Intro(engine);
  };
}

void Intro::enter() {}
void Intro::exit() {}

void Intro::pause() {}
void Intro::resume() {}

void Intro::update() {
  engine.popState();
  engine.pushState(Gameplay::makeState());

  ticktime += engine.ticktime.delta;
}

void Intro::draw() {}

Intro::Intro(Engine& engine) : GameState(engine) {}
}
