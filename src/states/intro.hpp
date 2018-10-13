#pragma once

#include "basestate.hpp"

namespace ke {
class Intro : public BaseState {
private:
  bool paused = true;

public:
  static Factory create();

  float ticktime = 0.f;

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  ~Intro() final = default;

private:
  Intro(Engine*);
};
}
