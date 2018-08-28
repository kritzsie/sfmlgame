#pragma once

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
}
