#include "goomba.hpp"
#include "../engine.hpp"
#include "../util/math.hpp"

namespace ke {
Goomba::Factory Goomba::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Goomba(engine, world);
  };
}

bool Goomba::isAlive() const {
  return alive;
}

bool Goomba::isAirborne() const {
  return airborne;
}

void Goomba::die() {
  alive = false;
}

void Goomba::think() {
  if (airborne == false) {
    if (alive == false) {
      vel.x = 0.f;
    }
    else if (std::abs(vel.x) < 32.f) {
      vel.x = std::copysign(32.f, vel.x);
    }
  }
  else if (valid == true) {
    if (pos.y + height <= 0.f) {
      valid = false;
    }
  }
}

void Goomba::applyGravity() {
  float gravity = world->getGravity();

  vel.y = std::max(-max_vel.y, std::min(vel.y + gravity / engine->ticktime.rate, max_vel.y));
}

void Goomba::resolveEntityCollisions() {
  for (BaseEntity* baseentity : world->getEntities()) {
    Entity* player = dynamic_cast<Player*>(baseentity);
    if (player != nullptr) {
      if (alive == true
      and player->getBBox().intersects(getBBox())) {
        die();
        engine->sound->play("stomp");
      }
    }
  }
}

void Goomba::resolveWorldCollisions() {
  airborne = true;

  Rect<int> range = World::tilesInBBox(getBBox());
  for (int y = range.y; y <= range.y + range.h; ++y)
  for (int x = range.x; x <= range.x + range.w; ++x) {
    Rect<float> entBox = getBBox();
    Rect<float> tileBox = World::tileBBox(x, y);

    if (x >= 0 and x < world->getSize().x
    and y >= 0 and y < world->getSize().y) {
      tileid_t tileid = world->getTile(x, y);
      if (tileid != 0
      and engine->getTileDef(tileid).type == TileType::SOLID
      and entBox.intersects(tileBox)) {
        Rect<float> collBox = entBox.intersection(tileBox);
        if (entBox.y + entBox.h / 2.f < tileBox.y + tileBox.h / 2.f) {
          vel.y = 0.f;
          pos.y -= collBox.h;
        }
        else {
          airborne = false;
          vel.y = 0.f;
          pos.y += collBox.h;
        }
        goto endfor;
      }
    }
  } endfor:

  range = World::tilesInBBox(getBBox());
  for (int y = range.y; y <= range.y + range.h; ++y)
  for (int x = range.x; x <= range.x + range.w; ++x) {
    Rect<float> entBox = getBBox();
    Rect<float> tileBox = World::tileBBox(x, y);

    if (x >= 0 and x < world->getSize().x
    and y >= 0 and y < world->getSize().y) {
      tileid_t tileid = world->getTile(x, y);
      if (tileid != 0
      and entBox.intersects(tileBox)) {
        switch (engine->getTileDef(tileid).type) {
        case TileType::SOLID: {
          vel.x = -vel.x;
          return;
        }
        default:
          continue;
        }
      }
    }
  }
}

void Goomba::update() {
  timer += engine->ticktime.delta;

  think();
  applyGravity();

  pos += vel / engine->ticktime.rate;

  resolveEntityCollisions();
  resolveWorldCollisions();

  if (alive == true) {
    setState("walking", getFrameOffset(timer));
  }
  else {
    setState("death");
  }
}

Goomba::Goomba(Engine* engine, World* world)
: Entity(EntityType::Goomba, engine, world) {
  pushFrame("walking", "goombawalk0", Rect(0, 0, 16, 16), Vec2(8.f, -1.f), 0.25f);
  pushFrame("walking", "goombawalk1", Rect(0, 0, 16, 16), Vec2(8.f, -1.f), 0.25f);

  pushFrame("death", "goombastomp", Rect(0, 0, 16, 8), Vec2(8.f, 0.f), 0.f);

  setState("walking");

  vel.x = std::copysign(32.f, pos.x - world->player->pos.x);
}
}
