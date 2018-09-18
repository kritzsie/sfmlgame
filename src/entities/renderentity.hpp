#pragma once

#include "baseentity.hpp"
#include "../renderer.hpp"
#include "../types.hpp"

#include <cstddef>
#include <map>

namespace ke {
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
public:
  static Factory create();

protected:
  RenderEntity(Engine*, World*);
};
}
