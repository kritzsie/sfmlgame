#pragma once

#include <functional>

namespace ke {
class Engine;

class GameState {
protected:
  Engine* const engine = nullptr;

public:
  using Factory = std::function<GameState*(Engine*)>;

  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;

  virtual void update() = 0;
  virtual void draw() = 0;

  virtual ~GameState() = default;

protected:
  GameState(Engine*);
};
}

