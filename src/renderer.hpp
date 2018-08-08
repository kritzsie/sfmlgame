#pragma once

#include <string>
#include <vector>

namespace ke {
struct RenderState {
  std::string texture;
  float duration;
};

using RenderStates = std::vector<RenderState>;
}
