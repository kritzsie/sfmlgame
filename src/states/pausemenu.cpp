#include "pausemenu.hpp"

#include "gameplay.hpp"
#include "../engine.hpp"

namespace ke {
PauseMenu::Factory PauseMenu::create(BaseState* previous) {
  return [previous](Engine* engine) -> BaseState* {
    return new PauseMenu(engine, previous);
  };
}

void PauseMenu::enter() {}
void PauseMenu::exit() {}

void PauseMenu::pause() {}
void PauseMenu::resume() {
  previous->resume();
}

void PauseMenu::update() {
  const Input& pause_input = engine->inputs[Action::pause];

  if (~pause_input > 0.f) {
    engine->sound->play("pause");
    engine->music->setVolume(1.f);
    engine->popState();
    resume();
  }
}

void PauseMenu::draw() {}

PauseMenu::PauseMenu(Engine* engine, BaseState* previous)
: BaseState(engine), previous(previous) {}
}
