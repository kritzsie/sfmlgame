#pragma once

#include "gamestate.hpp"

namespace ke {
class Gameplay : public GameState {
public:
  void enter() override;
  void exit() override;

  void pause() override;
  void resume() override;

  void update() override;
  void draw() override;
};
}
