#include "player.hpp"
#include "../engine.hpp"

#include <cmath>
#include <cstddef>

namespace ke {
Player::Factory Player::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Player(engine, world);
  };
}

void Player::resolveEntityCollisionsX() {}
void Player::resolveEntityCollisionsY() {}

void Player::resolveWorldCollisionsX() {
  Rect<int> range = World::tilesInBBox(getBBox());
  for (int x = range.x; x <= range.x + range.w; ++x)
  for (int y = range.y; y <= range.y + range.h; ++y) {
    Rect<float> plyrBox = getBBox();
    Rect<float> tileBox = World::tileBBox(x, y);

    if (x >= 0 and x < world->size.x
    and y >= 0 and y < world->size.y) {
      tileid_t tileid = world->getTile(x, y);
      if (tileid != 0
      and engine->getTileDef(tileid).type == TileType::SOLID
      and plyrBox.intersects(tileBox)) {
        Rect<float> collBox = plyrBox.intersection(tileBox);
        if (plyrBox.x + plyrBox.w / 2.f < tileBox.x + tileBox.w / 2.f) {
          vel.x = 0.f;
          pos.x -= collBox.w;
        }
        else {
          vel.x = 0.f;
          pos.x += collBox.w;
        }
        return;
      }
    }
  }
}

void Player::resolveWorldCollisionsY() {
  state |= airborne;

  Rect<int> range = World::tilesInBBox(getBBox());
  for (int x = range.x; x <= range.x + range.w; ++x)
  for (int y = range.y; y <= range.y + range.h; ++y) {
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
        return;
      }
    }
  }
}

void Player::jump(const Input& jump_input) {
  if (~jump_input > 0.f
  and !(state & airborne)
  and !(state & underwater)) {
    jumptime = 0.3125f;
    engine->sound->play("jump");
  }

  if (jumptime > 0.f) {
    vel.y = 176.f + std::abs(vel.x / 16.f);
    jumptime = std::max(0.f, jumptime - 1.f / engine->ticktime.rate);
  }
}

void Player::duck() {
  if (!(state & airborne)) {
    height = 15.f;
    state |= ducking;
  }
}

void Player::stand() {
  if (!(state & airborne)) {
    height = 25.f;
    state &= ~ducking;
  }
}

void Player::updateState() {
  if (state & airborne
  and !(state & ducking)) {
    setState("jumping");
  }
  else if (state & ducking) {
    setState("ducking");
  }
  else if (!(state & airborne)) {
    if (state & slipping) {
      setState("slipping");
    }
    else if (state & walking) {
      std::size_t offset = std::floor(
        std::fmod(
          walktime * getFrame().duration / engine->ticktime.rate,
          getFrameCount("walking")
      )
    );
      setState("walking", offset);
    }
    else {
      setState("idle");
    }
  }
}

void Player::update() {
  const Input& left_input = engine->inputs[Action::left];
  const Input& right_input = engine->inputs[Action::right];
  const Input& duck_input = engine->inputs[Action::down];

  const Input& run_input = engine->inputs[Action::run];
  const Input& jump_input = engine->inputs[Action::jump];

  if (jump_input > 0.f) {
    jump(jump_input);
  }
  else if (jumptime != 0.f) {
    jumptime = 0.f;
  }

  const float direction = std::max(-1.f, std::min(right_input - left_input, 1.f));
  if (duck_input > 0.f
  and direction == 0.f) {
    duck();
  }
  else {
    if (direction != 0.f) {
      setDirection(direction);
    }
    stand();
  }

  state &= ~ slipping;
  if ((vel.x != 0.f and !(state & airborne))
  or  direction != 0.f) {
    if ((direction > 0.f and vel.x < 0.f)
    or  (direction < 0.f and vel.x > 0.f)) {
      state |= slipping;
    }
    state |= walking;
    walktime += engine->ticktime.rate * std::min(
      std::abs(vel.x * 0.75f / engine->ticktime.rate) + 0.75f, 16.f
    );
  }
  else {
    state &= ~walking;
    walktime = 0.f;
  }

  const float max_run_vel = run_input ? 160.f : 128.f;
  if (direction > 0.f
  and vel.x >= 0.f
  and vel.x <= max_run_vel) {
    vel.x = std::min(max_vel.x, vel.x + direction * 192.f / engine->ticktime.rate);
  }
  else if (direction < 0.f
  and vel.x <= 0.f
  and vel.x >= -max_run_vel) {
    vel.x = std::max(-max_vel.x, vel.x + direction * 192.f / engine->ticktime.rate);
  }
  else if (direction < 0.f
  and vel.x > 0.f) {
    vel.x = std::max(0.f, vel.x - 320.f / engine->ticktime.rate);
  }
  else if (direction > 0.f
  and vel.x < 0.f) {
    vel.x = std::min(0.f, vel.x + 320.f / engine->ticktime.rate);
  }
  else if (!(state & airborne)) {
    if (vel.x > 0.f) {
      vel.x = std::max(0.f, vel.x - 256.f / engine->ticktime.rate);
    }
    else if (vel.x < 0.f) {
      vel.x = std::min(0.f, vel.x + 256.f / engine->ticktime.rate);
    }
  }

  if (jumptime == 0.f
  and vel.y > -max_vel.y) {
    vel.y = std::max(-max_vel.y, vel.y + world->gravity / engine->ticktime.rate);
  }

  resolveEntityCollisionsY();
  resolveEntityCollisionsX();

  pos.y += vel.y / engine->ticktime.rate;
  resolveWorldCollisionsY();

  pos.x += vel.x / engine->ticktime.rate;
  resolveWorldCollisionsX();

  updateState();
}

Player::Player(Engine* engine, World* world)
: Entity(engine, world, 5.f, 25.f), max_vel(192.f, 256.f) {
  pushFrame("idle", "bigmariowalk_0", Rect(0, 0, 14, 27), Vec2(7.f, -1.f), 0.f);

  pushFrame("walking", "bigmariowalk_1", Rect(0, 0, 16, 27), Vec2(9.f, -1.f), 0.125f);
  pushFrame("walking", "bigmariowalk_2", Rect(0, 0, 16, 26), Vec2(9.f, -1.f), 0.125f);
  pushFrame("walking", "bigmariowalk_1", Rect(0, 0, 16, 27), Vec2(9.f, -1.f), 0.125f);
  pushFrame("walking", "bigmariowalk_0", Rect(0, 0, 14, 27), Vec2(7.f, -1.f), 0.125f);

  pushFrame("slipping", "bigmarioslip", Rect(0, 0, 16, 28), Vec2(8.f, -1.f), 0.f);

  pushFrame("ducking", "bigmarioduck", Rect(0, 0, 14, 18), Vec2(7.f, -1.f), 0.f);

  pushFrame("jumping", "bigmariojump", Rect(0, 0, 16, 26), Vec2(8.f, -1.f), 0.f);
}
}
