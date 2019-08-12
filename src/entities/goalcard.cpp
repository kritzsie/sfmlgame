#include "goalcard.hpp"
#include "../engine.hpp"

namespace ke {
GoalCard::Factory GoalCard::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new GoalCard(engine, world);
  };
}

void GoalCard::update() {
  timer += engine->ticktime.delta;

  setState(getState(), getFrameOffset(timer));
}

GoalCard::GoalCard(Engine* engine, World* world)
: Entity(EntityType::GoalCard, engine, world) {
  pushFrame("idle", "redmushroom", Rect(0, 0, 16, 16), Vec2(8.f, 0.f), 0.125f);
  pushFrame("idle", "fireflower", Rect(0, 0, 16, 16), Vec2(8.f, 0.f), 0.125f);
  pushFrame("idle", "superstar_0", Rect(0, 0, 16, 16), Vec2(8.f, 0.f), 0.125f);
}
}
