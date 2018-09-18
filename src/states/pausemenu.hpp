#pragma once

#include "gamestate.hpp"

namespace ke {
class PauseMenu : public GameState {
private:
  GameState* const previous;

public:
  static Factory create(GameState*);

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  ~PauseMenu() final = default;

private:
  PauseMenu(Engine*, GameState*);
};
}
