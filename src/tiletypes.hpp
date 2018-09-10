#pragma once

#include "geometry.hpp"
#include "renderer.hpp"
#include "types.hpp"

#include <cstddef>
#include <cstdint>

namespace ke {
enum struct TileType : char {
  NONE, SOLID, LIQUID, PLATFORM, SLOPE
};

enum struct TileEdge : char {
  NONE, SOLID, HURT, KILL
};

struct TileSides {
  TileEdge left, right, down, up;
};

class TileDef {
public:
  renderer::Frames frames;

  std::size_t getFrameOffset(float) const;

  void addFrame(std::string, Vec2i, float);
};
}
