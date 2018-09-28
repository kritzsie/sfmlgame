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

class RenderStates {
private:
  std::string state = "idle";
  std::size_t offset = 0;
  std::map<std::string, RenderFrames> states;

public:
  void pushFrame(std::string, RenderFrame);
  void pushFrame(std::string, std::string, Vec2i, Vec2f, float);
  void pushFrame(std::string, std::string, Rect<int>, Vec2f, float);

  void setState(std::string);
  void setState(std::string, std::size_t);

  StringList getStateList() const;
  std::size_t getFrameCount(std::string) const;
  std::size_t getFrameOffset(float) const;

  const std::string& getState() const;
  const RenderFrame& getFrame() const;
};
}
