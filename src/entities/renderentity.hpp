#pragma once

#include "baseentity.hpp"
#include "../renderer.hpp"
#include "../types.hpp"

#include <cstddef>
#include <map>

namespace ke {
class RenderStates {
private:
  std::string state;
  std::size_t offset;
  std::map<std::string, RenderFrames> frames;

public:
  void pushFrame(std::string, RenderFrame);
  void pushFrame(std::string, std::string, Rect<int>, Vec2f, float);

  void setState(std::string, std::size_t);

  StringList getStateList() const;
  std::size_t getFrameCount(std::string) const;

  const std::string& getState() const;
  const RenderFrame& getFrame() const;

  RenderStates();
};

class Sprite : public RenderStates {
private:
  byte direction = 1;

public:
  Vec2f scale;

  byte getDirection() const;
  void setDirection(byte);

  Sprite();
};

class RenderEntity : public BaseEntity, public Sprite {
private:
  float time = 0.f;

public:
  static Factory create();

  void update() override;

  ~RenderEntity() override = default;

protected:
  RenderEntity(Engine*, World*);
};
}
