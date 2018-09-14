#include "tiletypes.hpp"

#include "renderer.hpp"

#include <cstddef>
#include <ctgmath>

namespace ke {
void TileDef::pushFrame(std::string texture, Vec2i offset, float duration) {
  frames.push_back(RenderFrame{texture, Rect<int>(offset, Vec2(16)), Vec2(0.f, 0.f), duration});
}

std::size_t TileDef::getFrameCount() const {
  return frames.size();
}

// TODO: This method has GREAT potential for optimization
std::size_t TileDef::getFrameOffset(float time) const {
  float time_max = 0.f;

  for (auto frame : frames) {
    time_max += frame.duration;
  }

  float time_mod = std::fmod(time, time_max);
  float accumulator = 0.f;
  std::size_t counter = 0;

  while (time_mod >= accumulator) {
    accumulator += frames[counter++].duration;
  }

  return (counter - 1) % frames.size();
}

const RenderFrame& TileDef::getFrame(std::size_t index) const {
  return frames.at(index);
}

TileDef::TileDef(TileType type, TileSides sides) : type(type), sides(sides) {}

TileDef::TileDef(TileType type) : TileDef(type, TileSides{TileSide::SOLID}) {}

TileDef::TileDef() : TileDef(TileType::SOLID) {}
}
