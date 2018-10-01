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
  for (int y = range.y; y <= range.y + range.h; ++y)
  for (int x = range.x; x <= range.x + range.w; ++x) {
    Rect<float> plyrBox = getBBox();
    Rect<float> tileBox = World::tileBBox(x, y);

    if (x >= 0 and x < world->getSize().x
    and y >= 0 and y < world->getSize().y) {
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

  Rect<float> plyrBBox = getBBox();
  if (plyrBBox.x < 0.f) {
    vel.x = 0.f;
    pos.x -= plyrBBox.x;
  }
  else if (plyrBBox.x + plyrBBox.w > (world->getSize().x) * 16.f) {
    vel.x = 0.f;
    pos.x -= plyrBBox.x + plyrBBox.w - (world->getSize().x) * 16.f;
  }
}

void Player::resolveWorldCollisionsY() {
  state |= State::airborne;

  Rect<int> range = World::tilesInBBox(getBBox());
  for (int y = range.y; y <= range.y + range.h; ++y)
  for (int x = range.x; x <= range.x + range.w; ++x) {
    Rect<float> plyrBox = getBBox();
    Rect<float> tileBox = World::tileBBox(x, y);

    if (x >= 0 and x < world->getSize().x
    and y >= 0 and y < world->getSize().y) {
      tileid_t tileid = world->getTile(x, y);
      if (tileid != 0
      and engine->getTileDef(tileid).type == TileType::SOLID
      and plyrBox.intersects(tileBox)) {
        Rect<float> collBox = plyrBox.intersection(tileBox);
        if (plyrBox.y + plyrBox.h / 2.f < tileBox.y + tileBox.h / 2.f) {
          if (vel.y > 0.f) {
            engine->sound->play("bump");
          }

          jump_timer = 0.f;
          vel.y = 0.f;
          pos.y -= collBox.h;
        }
        else {
          state &= ~State::airborne;
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
  and !(state & State::airborne)
  and !(state & State::underwater)) {
    jump_timer = 0.3125f;
    engine->sound->play("jump");
  }

  if (jump_timer > 0.f) {
    vel.y = std::max(vel.y, 176.f + std::abs(vel.x / 8.f));
    jump_timer = std::max(0.f, jump_timer - 1.f / engine->ticktime.rate);
  }
}

void Player::duck() {
  if (!(state & State::airborne)) {
    height = 15.f;
    state |= State::ducking;
  }
}

void Player::stand() {
  if (!(state & State::airborne)) {
    height = 25.f;
    state &= ~State::ducking;
  }
}

void Player::die() {
  if (death_timer >= 0.75f
  and pos.y + height > 0.f) {
    vel.y = std::max(-max_vel.y, vel.y + world->gravity / engine->ticktime.rate);
    pos += vel / engine->ticktime.rate;
  }

  death_timer += 1.f / engine->ticktime.rate;
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
  else if (jump_timer != 0.f) {
    jump_timer = 0.f;
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

  if (direction != 0.f
  and !(state & State::airborne)
  and std::abs(vel.x) >= 152.f) {
    if (std::abs(vel.x) >= 184.f) {
      p_meter = 7.5f;
    }
    else {
      p_meter = std::min(8.f, p_meter + 6.f / engine->ticktime.rate);
    }
  }
  else if (p_meter > 0.f) {
    if (!(state & State::airborne)
    or  p_meter < 7.f) {
      p_meter = std::max(0.f, p_meter - 2.5f / engine->ticktime.rate);
    }
  }

  if (p_meter >= 7.f) {
    if (run_timer > 0.f) {
      run_timer -= 1.f / engine->ticktime.rate;
    }
    else {
      run_timer = 0.1f;
      engine->sound->play("running");
    }
  }
  else if (run_timer > 0.f) {
    run_timer = 0.f;
  }

  state &= ~(State::walking | State::turning);
  if (!(state & State::airborne)) {
    state &= ~State::running;
  }
  if ((direction != 0.f or vel.x != 0.f)
  and !(state & State::airborne)) {
    if ((direction > 0.f and vel.x < 0.f)
    or  (direction < 0.f and vel.x > 0.f)) {
      state |= State::turning;
      if (turn_timer == 0.f) {
        turn_timer = 0.125;
        engine->sound->play("slip");
      }

      turn_timer = std::max(0.f, turn_timer - 1.f / engine->ticktime.rate);
    }
    else if (turn_timer > 0.f) {
      turn_timer = 0.f;
    }

    if (std::abs(vel.x) >= 192.f) {
      state |= State::running;
      state_timer += 1.f / engine->ticktime.rate;
    }
    else {
      state |= State::walking;
      state_timer += (std::abs(vel.x) / 96.f + 1.f) / engine->ticktime.rate;
    }
  }
  else {
    state_timer = 0.f;
  }

  const float max_run_vel = (run_input ? (p_meter > 7.f ? 192.f : 160.f) : 96.f);
  if (direction > 0.f
  and vel.x >= 0.f
  and vel.x <= max_run_vel) {
    vel.x = std::min(max_vel.x, vel.x + direction * 224.f / engine->ticktime.rate);
  }
  else if (direction < 0.f
  and vel.x <= 0.f
  and vel.x >= -max_run_vel) {
    vel.x = std::max(-max_vel.x, vel.x + direction * 224.f / engine->ticktime.rate);
  }
  else if (direction < 0.f
  and vel.x > 0.f) {
    vel.x = std::max(0.f, vel.x - 448.f / engine->ticktime.rate);
  }
  else if (direction > 0.f
  and vel.x < 0.f) {
    vel.x = std::min(0.f, vel.x + 448.f / engine->ticktime.rate);
  }
  else if (!(state & State::airborne)) {
    if (vel.x > 0.f) {
      vel.x = std::max(0.f, vel.x - 224.f / engine->ticktime.rate);
    }
    else if (vel.x < 0.f) {
      vel.x = std::min(0.f, vel.x + 224.f / engine->ticktime.rate);
    }
  }

  if (jump_timer == 0.f
  and vel.y > -max_vel.y) {
    vel.y = std::max(-max_vel.y, vel.y + world->gravity / engine->ticktime.rate);
  }

  resolveEntityCollisionsY();
  resolveEntityCollisionsX();

  pos.y += vel.y / engine->ticktime.rate;
  resolveWorldCollisionsY();

  pos.x += vel.x / engine->ticktime.rate;
  resolveWorldCollisionsX();

  if (pos.y + height <= 0.f) {
    state |= State::dead;
    death_timer = 0.f;
    vel.x = 0.f;
    vel.y = 256.f;
  }

  updateState();
}

void Player::updateState() {
  if (state & State::dead) {
    setState("death");
  }
  else if (state & State::ducking) {
    setState("ducking");
  }
  else if (state & State::airborne) {
    if (p_meter >= 7.f) {
      setState("runjumping");
    }
    else {
      setState("jumping");
    }
  }
  else if (state & State::running) {
    setState("running", getFrameOffset(state_timer));
  }
  else if (!(state & State::airborne)) {
    if (state & State::turning) {
      setState("slipping");
    }
    else if (state & State::walking) {
      setState("walking", getFrameOffset(state_timer));
    }
    else {
      setState("idle");
    }
  }
}

Player::Player(Engine* engine, World* world)
: Entity(engine, world, 5.f, 25.f), max_vel(192.f, 256.f) {
  pushFrame("idle", "bigmariowalk_0", Rect(0, 0, 14, 27), Vec2f(7, -1), 0.f);

  pushFrame("walking", "bigmariowalk_1", Rect(0, 0, 16, 27), Vec2f(9, -1), 0.125f);
  pushFrame("walking", "bigmariowalk_2", Rect(0, 0, 16, 26), Vec2f(9, 0), 0.125f);
  pushFrame("walking", "bigmariowalk_1", Rect(0, 0, 16, 27), Vec2f(9, -1), 0.125f);
  pushFrame("walking", "bigmariowalk_0", Rect(0, 0, 14, 27), Vec2f(7, -1), 0.125f);

  pushFrame("running", "bigmariorun_1", Rect(0, 0, 19, 27), Vec2f(11, -1), 1.f / 60.f);
  pushFrame("running", "bigmariorun_2", Rect(0, 0, 19, 26), Vec2f(11, 0), 1.f / 60.f);
  pushFrame("running", "bigmariorun_1", Rect(0, 0, 19, 27), Vec2f(11, -1), 1.f / 60.f);
  pushFrame("running", "bigmariorun_0", Rect(0, 0, 19, 27), Vec2f(11, -1), 1.f / 60.f);

  pushFrame("slipping", "bigmarioslip", Rect(0, 0, 16, 28), Vec2f(8, -1), 0.f);

  pushFrame("runjumping", "bigmariorunjump", Rect(0, 0, 19, 26), Vec2f(11, 0), 0.f);

  pushFrame("ducking", "bigmarioduck", Rect(0, 0, 14, 18), Vec2f(7, -1), 0.f);

  pushFrame("jumping", "bigmariojump", Rect(0, 0, 16, 26), Vec2f(8, 0), 0.f);

  pushFrame("death", "mariodeath", Rect(0, 0, 16, 16), Vec2f(8, -1), 0.f);
}
}
