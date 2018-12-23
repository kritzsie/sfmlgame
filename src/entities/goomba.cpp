#include "goomba.hpp"
#include "../engine.hpp"
#include "../util/math.hpp"

namespace ke {
Goomba::Factory Goomba::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Goomba(engine, world);
  };
}

void Goomba::update() {
  timer += engine->ticktime.delta;

  for (BaseEntity* baseentity : world->getEntities()) {
    Entity* player = dynamic_cast<Player*>(baseentity);
    if (player != nullptr) {
      if (state != State::dead
      and getBBox().intersects(player->getBBox())) {
        state = State::dead;
        engine->sound->play("stomp");
      }
    }
  }

  switch (state) {
  case State::alive:
    setState("walking", getFrameOffset(timer));
    break;
  case State::dead:
    setState("death");
    break;
  }
}

Goomba::Goomba(Engine* engine, World* world)
: Entity(engine, world) {
  setState("walking");

  pushFrame("walking", "goombawalk0", Rect(0, 0, 16, 16), Vec2(8.f, -1.f), 0.25f);
  pushFrame("walking", "goombawalk1", Rect(0, 0, 16, 16), Vec2(8.f, -1.f), 0.25f);

  pushFrame("death", "goombastomp", Rect(0, 0, 16, 8), Vec2(8.f, 0.f), 0.f);
}
}
