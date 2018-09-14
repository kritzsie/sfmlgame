#include "player.hpp"
#include "../engine.hpp"

#include <cmath>

namespace ke {
Player::Factory Player::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Player(engine, world);
  };
}

void Player::resolveEntityCollisions() {}

void Player::resolveWorldCollisions() {
  Rect<int> range = World::tilesInBBox(getBBox());

  if (vel.y) {
    pos.y += vel.y / engine->ticktime.rate;
  }
  //airborne = true;

  for (int x = range.x; x < range.x + range.w + 1; x++)
  for (int y = range.y; y < range.y + range.h + 1; y++) {
    Rect<float> plyrBox = getBBox();
    Rect<float> tileBox = World::tileBBox(x, y);

    if (x >= 0 and x < world->size.x
    and y >= 0 and y < world->size.y) {
      tileid_t tileid = world->getTile(x, y);
      if (tileid != 0
      and engine->getTileDef(tileid).type == TileType::SOLID
      and plyrBox.intersects(tileBox)) {
        Rect<float> collBox = plyrBox.intersection(tileBox);

        if (plyrBox.y + plyrBox.h / 2.f < tileBox.y + tileBox.h / 2.f) {
          if (pos.x > tileBox.x + tileBox.w
          and world->getTile(x + 1, y) == 1) {
            world->setTile(x + 1, y, 0);
          }
          else {
            world->setTile(x, y, 0);
          }
          engine->sound->play("brickshatter");
          if (vel.y > 0.f) {
            engine->sound->play("bump");
          }
          jumptime = 0.f;
          vel.y = 0.f;
          pos.y -= collBox.h;
        }
        else {
          state &= ~airborne;
          vel.y = 0.f;
          pos.y += collBox.h;
        }
      }
    }
  }
}

void Player::update() {
  if (vel.y > -max_vel.y) {
    vel.y = std::max(
      -max_vel.y,
      vel.y + world->gravity / engine->ticktime.rate
    );
  }
  pos += vel / engine->ticktime.rate;

  state |= airborne;

  resolveEntityCollisions();
  resolveWorldCollisions();

  if (state & airborne) {
    setState("jumping", 0);
  }
  else {
    setState("idle", 0);
  }
}

Player::Player(Engine* engine, World* world)
: Entity(engine, world, 5.f, 26.f), max_vel(0.f, 192.f) {
  pushFrame("idle", "bigmariowalk_0", Rect(0, 0, 14, 27), Vec2(7.f, -1.f), 0.f);
  pushFrame("jumping", "bigmariojump", Rect(0, 0, 16, 26), Vec2(8.f, -1.f), 0.f);
}
}
