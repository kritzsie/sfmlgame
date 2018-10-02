#pragma once

#include "basestate.hpp"

namespace ke {
class PauseMenu : public BaseState {
private:
  BaseState* const previous;

public:
  static Factory create(BaseState*);

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  ~PauseMenu() final = default;

private:
  PauseMenu(Engine*, BaseState*);
};
}
