#pragma once

#include "gamestate.hpp"
#include "../engine.hpp"

namespace ke {
class Intro : public GameState {
public:
  static GameState::Factory makeState();

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  ~Intro() final = default;

private:
  Intro(Engine*);
};
}
