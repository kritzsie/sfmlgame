#include "gameplay.hpp"

#include "../assetmanager.hpp"

namespace ke {
GameState::Factory Gameplay::makeState() {
  return [](Engine* engine) -> GameState* {
    return new Gameplay(engine);
  };
}

void Gameplay::drawBG(uint32_t color) {
  engine->viewport->clear(sf::Color(color));
}

void Gameplay::drawBG(std::string bg, Vec2f dist) {
  auto win_w = engine->viewport->getSize().x;
  auto win_h = engine->viewport->getSize().y;

  const auto& texture = assets::gfx.getTexture(bg);

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera.pos.x - win_w / 2 / engine->fbscale;
  float bottom = world->camera.pos.y - win_h / 2 / engine->fbscale;
  int sky_w = texture.getSize().x;
  int sky_h = texture.getSize().y;
  int min_x = left / sky_w;
  int min_y = bottom / sky_h;
  int max_x = (world->camera.pos.x + win_w * (2 * dist.x - 1) / 2 / engine->fbscale) / sky_w + dist.x;
  int max_y = (world->camera.pos.y + win_h * (2 * dist.y - 1) / 2 / engine->fbscale) / sky_h + dist.y;

  sf::Sprite sky(texture);
  for (int y = std::floor(min_y / dist.y); y < std::floor(max_y / dist.y) + 1; y++)
  for (int x = std::floor(min_x / dist.x); x < std::floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2f(x * sky_w + left / distdivx + 16 * world->padding.left / dist.x, y * sky_h + sky_h + bottom / distdivy - 16 * world->padding.top / dist.y)));
    engine->viewport->draw(sky);
  }
}

void Gameplay::drawBGBottom(std::string bg, Vec2f dist) {
  auto win_w = engine->viewport->getSize().x;
  auto win_h = engine->viewport->getSize().y;

  const auto& texture = assets::gfx.getTexture(bg);

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera.pos.x - win_w / 2 / engine->fbscale;
  float bottom = world->camera.pos.y - win_h / 2 / engine->fbscale;
  int sky_w = texture.getSize().x;
  int sky_h = texture.getSize().y - 11;
  int min_x = left / sky_w;
  int max_x = (world->camera.pos.x + win_w * (2 * dist.x - 1) / 2 / engine->fbscale) / sky_w + dist.x;

  sf::Sprite sky(texture);
  for (int x = std::floor(min_x / dist.x); x < std::floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2f(x * sky_w + left / distdivx + 16 * world->padding.left / dist.x, sky_h + bottom / distdivy + 16 * world->padding.bottom / dist.y)));
    engine->viewport->draw(sky);
  }
}

void Gameplay::drawBGTop(std::string bg, Vec2f dist) {
  auto win_w = engine->viewport->getSize().x;
  auto win_h = engine->viewport->getSize().y;

  const auto& texture = assets::gfx.getTexture(bg);

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera.pos.x - win_w / 2 / engine->fbscale;
  float top = world->camera.pos.y + win_h / 2 / engine->fbscale;
  int sky_w = texture.getSize().x;
  int min_x = left / sky_w;
  int max_x = (world->camera.pos.x + win_w * (2 * dist.x - 1) / 2 / engine->fbscale) / sky_w + dist.x;

  sf::Sprite sky(texture);
  for (int x = std::floor(min_x / dist.x); x < std::floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2f(x * sky_w + left / distdivx + 16 * world->padding.left / dist.x, world->size.y * 16 / dist.y + top / distdivy - 16 * world->padding.top / dist.y)));
    engine->viewport->draw(sky);
  }
}

void Gameplay::drawTiles() {
  auto win_w = engine->viewport->getSize().x;
  auto win_h = engine->viewport->getSize().y;

  int left = std::max(0, std::min(int((world->camera.pos.x - win_w / 2 / engine->fbscale) / 16), world->size.x));
  int bottom = std::max(0, std::min(int((world->camera.pos.y - win_h / 2 / engine->fbscale) / 16), world->size.y));
  int right = std::max(0, std::min(int((world->camera.pos.x + win_w / 2 / engine->fbscale) / 16) + 1, world->size.x));
  int top = std::max(0, std::min(int((world->camera.pos.y + win_h / 2 / engine->fbscale) / 16) + 1, world->size.y));

  for (int y = bottom; y < top; y++)
  for (int x = left; x < right; x++) {
    auto tileid = world->getTile(x, y);
    if (tileid) {
      const auto& texture = assets::gfx.getTile("smb3_tile_atlas");
      int xoffset = (tileid - 1) * 16 % texture.getSize().x;
      int yoffset = ((tileid - 1) / (texture.getSize().x / 16)) * 16 % texture.getSize().y;
      sf::Sprite tile(texture, sf::IntRect(xoffset, yoffset, 16, 16));
      tile.setPosition(World::toView(Vec2f(x * 16, y * 16 + 16)));
      engine->viewport->draw(tile);
    }
  }
}

void Gameplay::drawEntities() {
  world->player.sprite.setPosition(world->player.toView());
  world->player.sprite.setScale(sf::Vector2f(
    world->player.scale.x * world->player.getDirection(),
    world->player.scale.y
  ));
  engine->viewport->draw(world->player.sprite);
}

void Gameplay::drawUI() {}

void Gameplay::enter() {
  engine->music->change("overworld");
  engine->music->play();
}

void Gameplay::exit() {
  engine->music->stop();
}

void Gameplay::pause() {}
void Gameplay::resume() {}

void Gameplay::update() {
  // TODO: Send these to a better place
  const float gravity = -48 * 16;
  const float min_yvel = -16 * 16;

  // Handle keyboard input first
  auto up = engine->keys.up.getState();
  auto left = engine->keys.left.getState();
  auto down = engine->keys.down.getState();
  auto right = engine->keys.right.getState();
  auto jump = engine->keys.jump.getState();
  auto run = engine->keys.run.getState();

  auto direction = (right ? 1 : 0) + (left ? -1 : 0);

  float max_xvel = (run ? (world->player.p_speed ? 12 : 10) : 6) * 16;
  float min_xvel = -max_xvel;

  if (direction) {
    world->player.setDirection(direction);
  }

  if (not world->player.airborne) {
    if (direction and not world->player.ducking) {
      if (direction > 0
      and world->player.vel.x < max_xvel) {
        world->player.vel.x = std::max(-8.0f * 16.0f, std::min(world->player.vel.x + direction * 16 * 16 / engine->ticktime.rate, max_xvel));
      }
      else if (direction < 0
      and world->player.vel.x > min_xvel) {
        world->player.vel.x = std::max(min_xvel, std::min(world->player.vel.x + direction * 16 * 16 / engine->ticktime.rate, 8.0f * 16.0f));
      }
    }
    else if (world->player.vel.x > 0) {
      world->player.vel.x = std::max(world->player.vel.x - 12 * 16 / engine->ticktime.rate, 0.0f);
    }
    else if (world->player.vel.x < 0) {
      world->player.vel.x = std::min(world->player.vel.x + 12 * 16 / engine->ticktime.rate, 0.0f);
    }

    if (down and not direction) {
      world->player.sprite.setTexture(assets::gfx.getSprite("bigmarioduck"), true);
      world->player.offset.x = 7;
      world->player.duck();
    }
    else {
      if (world->player.vel.x > 0
      and direction < 0) {
        world->player.sprite.setTexture(assets::gfx.getSprite("bigmarioslip"), true);
        world->player.offset.x = 8;
        if (world->player.sliptime == 0) {
          world->player.sliptime = 0.09375;
          engine->sound->play("slip");
        }
        else {
          world->player.sliptime = std::max(0.0f, world->player.sliptime - 1 / engine->ticktime.rate);
        }
      }
      else if (world->player.vel.x < 0
      and direction > 0) {
        world->player.sprite.setTexture(assets::gfx.getSprite("bigmarioslip"), true);
        world->player.offset.x = 8;
        if (world->player.sliptime == 0) {
          world->player.sliptime = 0.09375;
          engine->sound->play("slip");
        }
        else {
          world->player.sliptime = std::max(0.0f, world->player.sliptime - 1 / engine->ticktime.rate);
        }
      }
      else {
        if (world->player.vel.x) {
          if (world->player.walktime) {
            world->player.walktime = std::max(0.0f, world->player.walktime - std::max(1.0f, std::abs(world->player.vel.x) / 32) / engine->ticktime.rate);
          }
          else {
            world->player.walktime = 0.125;
            world->player.walkcycle = (world->player.walkcycle + 1) % 4;
          }
        }
        else {
          world->player.walktime = 0;
          world->player.walkcycle = 0;
        }
        world->player.sprite.setTexture(assets::gfx.getSprite("bigmariowalk_" + std::to_string(world->player.walkcycle < 3 ? world->player.walkcycle : 1)), true);
        world->player.offset.x = world->player.walkcycle ? 9 : 7;
        world->player.sliptime = 0;
      }

      world->player.stand();
    }
  }
  else {
    if (direction) {
      if (direction > 0
      and world->player.vel.x < max_xvel) {
        world->player.vel.x = std::min(world->player.vel.x + direction * 10 * 16 / engine->ticktime.rate, max_xvel);
      }
      else if (direction < 0
      and world->player.vel.x > min_xvel) {
        world->player.vel.x = std::max(min_xvel, world->player.vel.x + direction * 10 * 16 / engine->ticktime.rate);
      }
    }
    if (not world->player.ducking) {
      world->player.sprite.setTexture(assets::gfx.getSprite("bigmariojump"), true);
      world->player.offset.x = 8;
    }
  }

  if (not world->player.underwater) {
    if (engine->keys.jump.getDelta() == 1) {
      if (not world->player.airborne) {
        world->player.jumptime = 0.3125;
        engine->sound->play("jump");
      }
    }
    else if (engine->keys.jump.getDelta() == -1) {
      world->player.jumptime = 0;
    }

    if (jump and world->player.jumptime > 0) {
      world->player.jumptime = std::max(world->player.jumptime - 1 / engine->ticktime.rate, 0.0f);
      world->player.jump();
    }
  }

  if (world->player.vel.y > min_yvel
  and world->player.jumptime == 0) {
    world->player.vel.y = std::max(world->player.vel.y + gravity / engine->ticktime.rate, min_yvel);
  }

  auto range = World::tilesFromAABB(world->player.getAABB());

  if (world->player.vel.y) {
    world->player.pos.y += world->player.vel.y / engine->ticktime.rate;
  }
  world->player.airborne = true;

  for (int x = range.x; x < range.x + range.w + 1; x++)
  for (int y = range.y; y < range.y + range.h + 1; y++) {
    auto plyrBox = world->player.getAABB();
    auto tileBox = World::tileAABB(x, y);

    if (x >= 0 and x < world->size.x
    and y >= 0 and y < world->size.y) {
      if (world->getTile(x, y) != 0
      and plyrBox.intersects(tileBox)) {
        auto collBox = plyrBox.intersection(tileBox);

        if (plyrBox.y + plyrBox.h / 2 < tileBox.y + tileBox.h / 2) {
          /*if (world->player.pos.x > tileBox.x + tileBox.w
          and world->getTile(x + 1, y) != 0) {
            world->setTile(x + 1, y, 0);
          }
          else {
            world->setTile(x, y, 0);
          }
          engine->sound->play("brickshatter");*/
          if (world->player.vel.y > 0) {
            engine->sound->play("bump");
          }
          world->player.jumptime = 0;
          world->player.vel.y = 0;
          world->player.pos.y -= collBox.h;
        }
        else {
          world->player.airborne = false;
          world->player.vel.y = 0;
          world->player.pos.y += collBox.h;
        }
      }
    }
  }

  if (world->player.vel.x) {
    world->player.pos.x += world->player.vel.x / engine->ticktime.rate;
  }

  for (int x = range.x; x < range.x + range.w + 1; x++)
  for (int y = range.y; y < range.y + range.h + 1; y++) {
    auto plyrBox = world->player.getAABB();
    auto tileBox = World::tileAABB(x, y);

    if (x >= 0 and x < world->size.x
    and y >= 0 and y < world->size.y) {
      if (world->getTile(x, y) != 0
      and plyrBox.intersects(tileBox)) {
        auto collBox = plyrBox.intersection(tileBox);

        if (plyrBox.x + plyrBox.w / 2 < tileBox.x + tileBox.w / 2) {
          world->player.vel.x = 0;
          world->player.pos.x -= collBox.w;
        }
        else {
          world->player.vel.x = 0;
          world->player.pos.x += collBox.w;
        }
      }
    }
  }

  world->camera.vel = (world->player.pos + Vec2f(0, world->player.height / 2) - world->camera.pos) * engine->ticktime.rate / 8.0f;

  world->camera.pos += world->camera.vel / engine->ticktime.rate;

  auto camLeft = engine->viewport->getSize().x / 2 / engine->fbscale + world->padding.left * 16;
  auto camRight = world->size.x * 16 - engine->viewport->getSize().x / 2 / engine->fbscale - world->padding.right * 16;
  auto camUp = world->size.y * 16 - engine->viewport->getSize().y / 2 / engine->fbscale - world->padding.top * 16;
  auto camDown = engine->viewport->getSize().y / 2 / engine->fbscale + world->padding.bottom * 16;

  if (world->camera.pos.x < camLeft
  and world->camera.pos.x > camRight) {
    world->camera.pos.x = (camLeft + camRight) / 2;
  }
  else {
    if (world->camera.pos.x < camLeft) {
      world->camera.pos.x = camLeft;
    }
    else if (world->camera.pos.x > camRight) {
      world->camera.pos.x = camRight;
    }
  }

  if (world->camera.pos.y > camUp
  and world->camera.pos.y < camDown) {
    world->camera.pos.y = (camUp + camDown) / 2;
  }
  else {
    if (world->camera.pos.y > camUp) {
      world->camera.vel.y = 0;
      world->camera.pos.y = camUp;
    }
    else if (world->camera.pos.y < camDown) {
      world->camera.vel.y = 0;
      world->camera.pos.y = camDown;
    }
  }

  engine->tickKeys();
}

void Gameplay::draw() {
  auto win_w = engine->viewport->getSize().x;
  auto win_h = engine->viewport->getSize().y;

  sf::View view(sf::Vector2f(), sf::Vector2f(std::floor(win_w / engine->fbscale), std::floor(win_h / engine->fbscale)));
  view.setCenter(World::toView(world->camera.pos));
  engine->viewport->setView(view);

  drawBG(0x6898F8FF);
  drawBGBottom("overworldblockstop", Vec2f(1.625, 1.375));
  drawBGTop("cloudlayer", Vec2f(2.625, 1.125));
  drawTiles();
  drawEntities();
  drawUI();

  engine->viewport->display();
}

Gameplay::Gameplay(Engine* engine) : GameState(engine) {
  world = new World(176, 27);
}

Gameplay::~Gameplay() {
  if (world != nullptr) delete world;
}
}
