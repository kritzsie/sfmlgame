#pragma once

#include "geometry.hpp"
#include "types.hpp"

#include <string>
#include <vector>

namespace ke {
struct RenderFrame {
  std::string texture;
  Rect<int> cliprect;
  Vec2f offset;
  float duration;
};

using RenderFrames = std::vector<RenderFrame>;
}
