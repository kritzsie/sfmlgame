#include "intro.hpp"

#include "basegame.hpp"
#include "../engine.hpp"

namespace ke {
Intro::Factory Intro::create() {
  return [](Engine* engine) -> BaseState* {
    return new Intro(engine);
  };
}

void Intro::enter() {
  resume();
}

void Intro::exit() {}

void Intro::pause() {
  paused = true;
}

void Intro::resume() {
  paused = false;
}

void Intro::update() {
  if (not paused) {
    engine->popState();
    engine->pushState(BaseGame::create());
    pause();
  }
}

void Intro::draw() {}

Intro::Intro(Engine* engine) : BaseState(engine) {}
}
