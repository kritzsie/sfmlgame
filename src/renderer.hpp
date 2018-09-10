#pragma once

#include "geometry.hpp"

#include <string>
#include <vector>

namespace ke::renderer {
struct Frame {
  std::string texture;
  Rect<float> cliprect;
  float duration;
};

using Frames = std::vector<Frame>;
}
