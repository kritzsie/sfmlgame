#pragma once

#include "geometry.hpp"
#include "renderer.hpp"
#include "types.hpp"

#include <cstddef>
#include <cstdint>

namespace ke {
enum struct TileType : char {
  NONE, SOLID, LIQUID, SLOPE
};

enum struct TileSide : char {
  NONE, SOLID, HURT, KILL
};

struct TileSides {
  TileSide top, bottom, left, right;
};

class TileDef {
private:
  RenderFrames frames;

public:
  const TileType type;
  const TileSides sides;

  void pushFrame(std::string, Vec2i, float);

  std::size_t getFrameCount() const;
  std::size_t getFrameOffset(float) const;

  const RenderFrame& getFrame(std::size_t) const;

  TileDef();
  TileDef(TileType);
  TileDef(TileType, TileSides);
};
}
