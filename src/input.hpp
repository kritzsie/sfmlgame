#pragma once

#include <map>

namespace ke {
enum class Action {
  up, left, down, right,
  jump, run, spinjump,
  pause, back
};

class Input {
private:
  float delta = 0.f;
  float value = 0.f;

public:
  float getValue() const;
  void setValue(float);
  float getDelta() const;
  void update();

  float operator ~() const; // Same as getDelta()
  Input& operator =(float);

  operator float() const;

  Input() = default;
  Input(const Input&) = delete;
};

using InputMap = std::map<Action, Input>;
}
