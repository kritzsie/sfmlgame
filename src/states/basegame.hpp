#pragma once

#include "basestate.hpp"
#include "../engine.hpp"

namespace ke {
class BaseGame : public BaseState {
private:
  bool paused = true;

public:
  static Factory create();

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  ~BaseGame() final = default;

private:
  BaseGame(Engine*);
};
}
