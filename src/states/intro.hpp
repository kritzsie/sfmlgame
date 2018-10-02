#pragma once

#include "basestate.hpp"
#include "../engine.hpp"

namespace ke {
class Intro : public BaseState {
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
