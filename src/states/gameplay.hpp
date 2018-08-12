#pragma once

#include "gamestate.hpp"
#include "../engine.hpp"
#include "../entity.hpp"
#include "../world.hpp"

#include <vector>

namespace ke {
class Gameplay : public GameState {
private:
  std::vector<World> world;
  std::vector<BaseEntity*> entities;

public:
  void enter() override;
  void exit() override;

  void pause() override;
  void resume() override;

  void update(const TimeInfo&) override;
  void draw(const TimeInfo&) override;
};
}
