#pragma once

#include "baseentity.hpp"

namespace ke {
class Camera : public BaseEntity {
private:
  BaseEntity* target = nullptr;

  void resolveWorldCollisions();

public:
  static Factory create();

  BaseEntity* getTarget() const;
  void setTarget(BaseEntity*);

  void update() override;

protected:
  Camera(Engine*, World*);
};
}
