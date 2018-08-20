#pragma once

#include "gamestate.hpp"
#include "../engine.hpp"
#include "../entity.hpp"
#include "../world.hpp"

#include <vector>

namespace ke {
class Gameplay : public GameState {
private:
  World* world = nullptr;
  std::vector<Entity*> entities;

  void drawBG(uint32_t);
  void drawBG(std::string, Vec2f);
  void drawBGBottom(std::string, Vec2f);
  void drawBGTop(std::string, Vec2f);
  void drawTiles();
  void drawEntities();
  void drawUI();

public:
  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  Gameplay(Engine*);
  ~Gameplay() final;
};
}
