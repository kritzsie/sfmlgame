#pragma once

#include "gamestate.hpp"
#include "../engine.hpp"
#include "../world.hpp"

#include <vector>

namespace ke {
class Engine;

class Gameplay : public GameState {
private:
  World* world = nullptr;

  float ticktime = 0.f;
  float rendertime = 0.f;

  bool paused = true;

  void drawBG(uint32_t);
  void drawBG(std::string, Vec2f);
  void drawBGBottom(std::string, Vec2f);
  void drawBGTop(std::string, Vec2f);
  void drawTiles();
  void drawEntities();
  void drawUI();

public:
  static Factory create();

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  ~Gameplay() final;

private:
  Gameplay(Engine*);
};
}
