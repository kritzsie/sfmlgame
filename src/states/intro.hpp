#pragma once

#include "gamestate.hpp"

namespace ke {
class Intro : public GameState {
public:
  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  Intro(Engine*);
  ~Intro() final;
};
}
