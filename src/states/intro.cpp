#include "intro.hpp"

#include "gameplay.hpp"
#include "../engine.hpp"

namespace ke {
Intro::Factory Intro::create() {
  return [](Engine* engine) -> BaseState* {
    return new Intro(engine);
  };
}

void Intro::enter() {}
void Intro::exit() {}

void Intro::pause() {}
void Intro::resume() {}

void Intro::update() {
  engine->popState();
  engine->pushState(Gameplay::create());

  ticktime += engine->ticktime.delta;
}

void Intro::draw() {}

Intro::Intro(Engine* engine) : BaseState(engine) {}
}
