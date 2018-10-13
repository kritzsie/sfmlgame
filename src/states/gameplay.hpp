#pragma once

#include "basestate.hpp"
#include "../entities.hpp"
#include "../world.hpp"

#include <SFML/Graphics.hpp>

namespace ke {
class BaseGame;

std::string highASCII(std::string);

class TextStyle {
public:
  std::string font;
  Vec2<bool> align;

  TextStyle(bool, bool);
  TextStyle(std::string = "smb3_sbfont");
  TextStyle(std::string, bool, bool);
};

class Gameplay : public BaseState {
private:
  BaseGame* basegame = nullptr;
  World* world = nullptr;

  float ticktime = 0.f;
  float rendertime = 0.f;

  bool paused = true;

  void drawBG(uint32_t);
  void drawBG(std::string, Vec2f);
  void drawBGBottom(std::string, Vec2f);
  void drawBGTop(std::string, Vec2f);
  void drawTiles();
  void drawEntity(RenderEntity*);
  void drawEntities();
  void drawText(std::string, Vec2f);
  void drawText(std::string, Vec2f, TextStyle);
  void drawUI();

public:
  static Factory create(BaseGame*);

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  ~Gameplay() final;

private:
  Gameplay(Engine*, BaseGame*);
};
}
