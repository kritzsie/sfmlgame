#pragma once

#include "basestate.hpp"
#include "../engine.hpp"

namespace ke {
class BaseGame : public BaseState {
private:
  bool paused = true;

  ubyte coins = 0;
  ubyte lives = 5;
  ulong score = 0;

public:
  static Factory create();

  ubyte getCoins() const;
  ubyte getLives() const;
  ulong getScore() const;

  void addCoins(int);
  void addLives(int);
  void addScore(ulong);

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
