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
protected:
  RenderEntity(EntityType, Engine*, World*);
};
}
