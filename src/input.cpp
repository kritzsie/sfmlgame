#include "input.hpp"

namespace ke {
float Input::getValue() const {
  return value;
}

void Input::setValue(float new_value) {
  new_value = std::max(0.0f, std::min(new_value, 1.0f));
  delta = std::max(-1.0f, std::min(delta + new_value - value, 1.0f));
  value = new_value;
}

float Input::getDelta() const {
  return delta;
}

void Input::update() {
  delta = 0.0f;
}

Input& Input::operator =(float rhs) {
  setValue(rhs);

  return *this;
}

Input::operator float() const {
  return value;
}
}
