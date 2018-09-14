#pragma once

#include "baseentity.hpp"
#include "../renderer.hpp"

namespace ke {
class RenderEntity : public BaseEntity {
public:
  static Factory create();

  std::string getTexture() const;

  ~RenderEntity() override = default;

protected:
  RenderEntity(Engine*, World*);
};
}
