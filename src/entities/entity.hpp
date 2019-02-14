#pragma once

#include "baseentity.hpp"
#include "renderentity.hpp"
#include "../geometry.hpp"

namespace ke {
class Entity : public RenderEntity {
public:
  float radius;
  float height;

  Rect<float> getBBox() const;

protected:
  Entity(EntityType, Engine*, World*);
  Entity(EntityType, Engine*, World*, float, float);
};
}
