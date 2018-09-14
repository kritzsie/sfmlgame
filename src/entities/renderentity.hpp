#pragma once

#include "baseentity.hpp"
#include "../renderer.hpp"
#include "../types.hpp"

namespace ke {
class RenderEntity : public BaseEntity {
private:
  byte direction;

public:
  static Factory create();

  byte getDirection() const;
  void setDirection(byte);

  std::string getTexture() const;

  ~RenderEntity() override = default;

protected:
  RenderEntity(Engine*, World*);
};
}
