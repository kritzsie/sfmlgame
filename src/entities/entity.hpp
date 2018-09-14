#pragma once

#include "baseentity.hpp"
#include "renderentity.hpp"
#include "../geometry.hpp"

namespace ke {
class Entity : public RenderEntity {
public:
  static Factory create();

  float radius;
  float height;

  Rect<float> getBBox() const;
  Vec2f getSize() const;

  ~Entity() override = default;

protected:
  Entity(Engine*, World*);
  Entity(Engine*, World*, float, float);
};
}
