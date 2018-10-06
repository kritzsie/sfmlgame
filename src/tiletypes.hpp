#pragma once

#include "geometry.hpp"
#include "renderer.hpp"
#include "types.hpp"

#include <cstddef>
#include <cstdint>

namespace ke {
enum class TileType : ubyte {
  NONE, SOLID, LIQUID, SLOPE, GOLDCOIN
};

enum class TileSide : ubyte {
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
