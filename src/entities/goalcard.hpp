#pragma once

#include "entity.hpp"

namespace ke {
class GoalCard : public Entity {
public:
  static Factory create();

  void update() override;

protected:
  GoalCard(Engine*, World*);

private:
  float timer = 0.f;
};
}
