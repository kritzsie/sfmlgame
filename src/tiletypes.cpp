#include "tiletypes.hpp"

#include "renderer.hpp"

#include <cstddef>
#include <ctgmath>

namespace ke {
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

std::size_t TileDef::pushFrame(std::string texture, Vec2i offset, float duration) {
  frames.push_back(renderer::Frame{texture, Rect<int>(offset, Vec2(16)), duration});
  return frames.size() - 1;
}

const renderer::Frame& TileDef::getFrame(std::size_t index) const {
  return frames.at(index);
}

TileDef::TileDef(TileType type, TileSides sides) : type(type), sides(sides) {}

TileDef::TileDef(TileType type) : TileDef(type, TileSides{TileSide::SOLID}) {}

TileDef::TileDef() : TileDef(TileType::SOLID) {}
}
