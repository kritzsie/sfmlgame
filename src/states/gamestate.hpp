#pragma once

namespace ke {
struct TimeInfo;

class GameState {
public:
  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;

  virtual void update(const TimeInfo&) = 0;
  virtual void draw(const TimeInfo&) = 0;

  GameState() = delete;
  virtual ~GameState() = default;
};
}
