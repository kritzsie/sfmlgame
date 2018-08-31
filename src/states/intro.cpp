#include "intro.hpp"

#include "gameplay.hpp"

#include <cstdlib>

namespace ke {
void Intro::enter() {}
void Intro::exit() {}

void Intro::pause() {}
void Intro::resume() {}

void Intro::update() {}

void Intro::draw() {
  engine->states.back()->exit();
  engine->states.pop_back();

  engine->states.push_back(new Gameplay(engine));
  engine->states.back()->enter();

  delete this;
}

Intro::Intro(Engine* engine) : GameState(engine) {}
}
