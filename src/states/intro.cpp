#include "intro.hpp"

namespace ke {
void Intro::enter() {}
void Intro::exit() {}

void Intro::pause() {}
void Intro::resume() {}

void Intro::update() {}

void Intro::draw() {}

Intro::Intro(Engine* engine) : GameState(engine) {}
Intro::~Intro() {}
}
