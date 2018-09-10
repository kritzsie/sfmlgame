#pragma once

#include "geometry.hpp"

#include <string>
#include <vector>

namespace ke::renderer {
struct Frame {
  std::string texture;
  Rect<int> cliprect;
  float duration;
};

using Frames = std::vector<Frame>;
}
