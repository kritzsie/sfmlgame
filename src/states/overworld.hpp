#pragma once

#include "basestate.hpp"
#include "basegame.hpp"
#include "../types.hpp"

namespace ke {
class Overworld : public BaseState {
public:
  enum class State : byte {
    idle, starting, paused, moving,
    levelstart, levelclear, updating
  };

private:
  State state = State::idle;

  BaseGame* basegame = nullptr;

public:
  static Factory create(BaseGame*);

  void enter() override;
  void exit() override;

  void pause() override;
  void resume() override;

  void update() override;
  void draw() override;

private:
  Overworld(Engine*, BaseGame*);
};
}
