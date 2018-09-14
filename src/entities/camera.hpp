#pragma once

#include "baseentity.hpp"

namespace ke {
class Camera : public BaseEntity {
private:
  BaseEntity* target = nullptr;

public:
  static Factory create();

  BaseEntity* getTarget() const;
  void setTarget(BaseEntity*);

  void update() override;

  ~Camera() override = default;

protected:
  Camera(Engine*, World*);
};
}
