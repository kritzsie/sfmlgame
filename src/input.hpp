#pragma once

#include <map>

namespace ke {
enum class Actions {
  up, left, down, right,
  jump, run, spinjump,
  pause, back
};

class Input {
private:
  float delta = 0.0f;
  float value = 0.0f;

public:
  float getValue() const;
  void setValue(float);
  float getDelta() const;
  void update();

  Input& operator =(float);

  operator float() const;

  Input() = default;
  Input(const Input&) = delete;
};

using InputMap = std::map<Actions, Input>;
}
