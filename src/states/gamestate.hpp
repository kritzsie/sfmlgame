#pragma once

namespace ke {
class Engine;

class GameState {
protected:
  Engine* const engine;

public:
  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;

  virtual void update() = 0;
  virtual void draw() = 0;

  GameState() = delete;
  GameState(Engine*);
  virtual ~GameState() = default;
};
}
