#include "gameplay.hpp"

#include "../assetmanager.hpp"
#include "../tiletypes.hpp"

namespace ke {
Gameplay::Factory Gameplay::create() {
  return [](Engine* engine) -> GameState* {
    return new Gameplay(engine);
  };
}

void Gameplay::drawBG(uint32_t color) {
  engine->viewport->clear(sf::Color(color));
}

void Gameplay::drawBG(std::string bg, Vec2f dist) {
  uint win_w = engine->viewport->getSize().x;
  uint win_h = engine->viewport->getSize().y;

  const sf::Texture& texture = assets::gfx.getTexture(bg);

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera->pos.x - win_w / 2 / engine->viewscale;
  float bottom = world->camera->pos.y - win_h / 2 / engine->viewscale;
  int sky_w = texture.getSize().x;
  int sky_h = texture.getSize().y;
  int min_x = left / sky_w;
  int min_y = bottom / sky_h;
  int max_x = (world->camera->pos.x + win_w * (2 * dist.x - 1) / 2 / engine->viewscale) / sky_w + dist.x;
  int max_y = (world->camera->pos.y + win_h * (2 * dist.y - 1) / 2 / engine->viewscale) / sky_h + dist.y;

  sf::Sprite sky(texture);
  for (int y = std::floor(min_y / dist.y); y < std::floor(max_y / dist.y) + 1; y++)
  for (int x = std::floor(min_x / dist.x); x < std::floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2(x * sky_w + left / distdivx + 16.f * world->padding.left / dist.x, y * sky_h + sky_h + bottom / distdivy - 16.f * world->padding.top / dist.y)));
    engine->viewport->draw(sky);
  }
}

void Gameplay::drawBGBottom(std::string bg, Vec2f dist) {
  uint win_w = engine->viewport->getSize().x;
  uint win_h = engine->viewport->getSize().y;

  const sf::Texture& texture = assets::gfx.getTexture(bg);

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera->pos.x - win_w / 2 / engine->viewscale;
  float bottom = world->camera->pos.y - win_h / 2 / engine->viewscale;
  int sky_w = texture.getSize().x;
  int sky_h = texture.getSize().y - 11;
  int min_x = left / sky_w;
  int max_x = (world->camera->pos.x + win_w * (2 * dist.x - 1) / 2 / engine->viewscale) / sky_w + dist.x;

  sf::Sprite sky(texture);
  for (int x = std::floor(min_x / dist.x); x < std::floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2(x * sky_w + left / distdivx + 16.f * world->padding.left / dist.x, sky_h + bottom / distdivy + 16.f * world->padding.bottom / dist.y)));
    engine->viewport->draw(sky);
  }
}

void Gameplay::drawBGTop(std::string bg, Vec2f dist) {
  uint win_w = engine->viewport->getSize().x;
  uint win_h = engine->viewport->getSize().y;

  const sf::Texture& texture = assets::gfx.getTexture(bg);

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera->pos.x - win_w / 2 / engine->viewscale;
  float top = world->camera->pos.y + win_h / 2 / engine->viewscale;
  int sky_w = texture.getSize().x;
  int min_x = left / sky_w;
  int max_x = (world->camera->pos.x + win_w * (2 * dist.x - 1) / 2 / engine->viewscale) / sky_w + dist.x;

  sf::Sprite sky(texture);
  for (int x = std::floor(min_x / dist.x); x < std::floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2(x * sky_w + left / distdivx + 16.f * world->padding.left / dist.x, world->size.y * 16.f / dist.y + top / distdivy - 16.f * world->padding.top / dist.y)));
    engine->viewport->draw(sky);
  }
}

void Gameplay::drawTiles() {
  uint win_w = engine->viewport->getSize().x;
  uint win_h = engine->viewport->getSize().y;

  int left = std::max(0, std::min(int((world->camera->pos.x - win_w / 2 / engine->viewscale) / 16), world->size.x));
  int bottom = std::max(0, std::min(int((world->camera->pos.y - win_h / 2 / engine->viewscale) / 16), world->size.y));
  int right = std::max(0, std::min(int((world->camera->pos.x + win_w / 2 / engine->viewscale) / 16) + 1, world->size.x));
  int top = std::max(0, std::min(int((world->camera->pos.y + win_h / 2 / engine->viewscale) / 16) + 1, world->size.y));

  for (int y = bottom; y < top; y++)
  for (int x = left; x < right; x++) {
    tileid_t tileid = world->getTile(x, y);
    if (tileid) {
      const TileDef& tiledef = engine->getTileDef(tileid);
      const std::size_t frame = tiledef.getFrameOffset(rendertime);
      const std::string& texture = tiledef.getFrame(frame).texture;
      if (texture != "") {
        sf::Sprite tile(assets::gfx.getTile(texture), tiledef.getFrame(frame).cliprect);
        tile.setPosition(World::toView(Vec2(x * 16.f, y * 16.f + 16.f)));
        engine->viewport->draw(tile);
      }
    }
  }
}

void Gameplay::drawEntities() {
  for (BaseEntity* baseentity : world->entities) {
    RenderEntity* entity = dynamic_cast<RenderEntity*>(baseentity);
    if (entity != nullptr) {
      const std::string& texture = entity->getFrame().texture;
      sf::Sprite sprite(assets::gfx.getSprite(texture));
      sprite.setPosition(World::toView(entity));
      engine->viewport->draw(sprite);
    }
  }
}

void Gameplay::drawUI() {}

void Gameplay::enter() {
  engine->music->change("overworld");
  engine->music->play();
  paused = false;
}

void Gameplay::exit() {
  engine->music->stop();
}

void Gameplay::pause() {
  engine->sound->play("pause");
  engine->music->setVolume(0.5f);
  paused = true;
}

void Gameplay::resume() {
  engine->sound->play("pause");
  engine->music->setVolume(1.f);
  paused = false;
}

void Gameplay::update() {
  // TODO: Send these to a better place
  const float gravity = -48.f * 16.f;
  const float min_yvel = -16.f * 16.f;

  // Handle keyboard input first
  const Input& left = engine->inputs[Action::left];
  const Input& down = engine->inputs[Action::down];
  const Input& right = engine->inputs[Action::right];

  const Input& jump = engine->inputs[Action::jump];
  const Input& run = engine->inputs[Action::run];

  const Input& pause_input = engine->inputs[Action::pause];

  if (~pause_input > 0.f) {
    if (paused) {
      resume();
    }
    else {
      pause();
    }
  }

  if (not paused) {
    for (BaseEntity* entity : world->entities) {
      entity->update();
    }
  }

  /*if (not paused) {
    byte direction = (right ? 1 : 0) + (left ? -1 : 0);

    float max_xvel = (run ? (world->player.p_speed ? 12.f : 10.f) : 6.f) * 16.f;
    float min_xvel = -max_xvel;

    if (direction) {
      world->player.setDirection(direction);
    }

    if (not world->player.airborne) {
      if (direction and not world->player.ducking) {
        if (direction > 0
        and world->player.vel.x < max_xvel) {
          world->player.vel.x = std::max(-8.f * 16.f, std::min(world->player.vel.x + direction * 16 * 16 / engine->ticktime.rate, max_xvel));
        }
        else if (direction < 0
        and world->player.vel.x > min_xvel) {
          world->player.vel.x = std::max(min_xvel, std::min(world->player.vel.x + direction * 16 * 16 / engine->ticktime.rate, 8.f * 16.f));
        }
      }
      else if (world->player.vel.x > 0.f) {
        world->player.vel.x = std::max(world->player.vel.x - 12.f * 16.f / engine->ticktime.rate, 0.f);
      }
      else if (world->player.vel.x < 0.f) {
        world->player.vel.x = std::min(world->player.vel.x + 12.f * 16.f / engine->ticktime.rate, 0.f);
      }

      if (down and not direction) {
        world->player.sprite.setTexture(assets::gfx.getSprite("bigmarioduck"), true);
        world->player.offset.x = 7;
        world->player.duck();
      }
      else {
        if (world->player.vel.x > 0.f
        and direction < 0) {
          world->player.sprite.setTexture(assets::gfx.getSprite("bigmarioslip"), true);
          world->player.offset.x = 8.f;
          if (world->player.sliptime <= 0.f) {
            world->player.sliptime = 0.09375f;
            engine->sound->play("slip");
          }
          else {
            world->player.sliptime = std::max(0.f, world->player.sliptime - 1.f / engine->ticktime.rate);
          }
        }
        else if (world->player.vel.x < 0.f
        and direction > 0) {
          world->player.sprite.setTexture(assets::gfx.getSprite("bigmarioslip"), true);
          world->player.offset.x = 8.f;
          if (world->player.sliptime <= 0.f) {
            world->player.sliptime = 0.09375f;
            engine->sound->play("slip");
          }
          else {
            world->player.sliptime = std::max(0.f, world->player.sliptime - 1.f / engine->ticktime.rate);
          }
        }
        else {
          if (world->player.vel.x) {
            if (world->player.walktime) {
              world->player.walktime = std::max(0.f, world->player.walktime - std::max(1.f, std::abs(world->player.vel.x) / 32.f) / engine->ticktime.rate);
            }
            else {
              world->player.walktime = 0.125f;
              world->player.walkcycle = (world->player.walkcycle + 1) % 4;
            }
          }
          else {
            world->player.walktime = 0.f;
            world->player.walkcycle = 0.f;
          }
          world->player.sprite.setTexture(assets::gfx.getSprite("bigmariowalk_" + std::to_string(world->player.walkcycle < 3 ? world->player.walkcycle : 1)), true);
          world->player.offset.x = world->player.walkcycle ? 9 : 7;
          world->player.sliptime = 0.f;
        }

        world->player.stand();
      }
    }
    else {
      if (direction) {
        if (direction > 0
        and world->player.vel.x < max_xvel) {
          world->player.vel.x = std::min(world->player.vel.x + direction * 10.f * 16.f / engine->ticktime.rate, max_xvel);
        }
        else if (direction < 0
        and world->player.vel.x > min_xvel) {
          world->player.vel.x = std::max(min_xvel, world->player.vel.x + direction * 10.f * 16.f / engine->ticktime.rate);
        }
      }
      if (not world->player.ducking) {
        world->player.sprite.setTexture(assets::gfx.getSprite("bigmariojump"), true);
        world->player.offset.x = 8.f;
      }
    }

    if (not world->player.underwater) {
      if (~jump > 0) {
        if (not world->player.airborne) {
          world->player.jumptime = 0.3125f;
          engine->sound->play("jump");
        }
      }
      else if (jump == 0) {
        world->player.jumptime = 0.f;
      }

      if (world->player.jumptime > 0.f) {
        world->player.jumptime = std::max(0.f, world->player.jumptime - 1.f / engine->ticktime.rate);
        world->player.jump();
      }
    }

    if (world->player.vel.y > min_yvel
    and world->player.jumptime == 0.f) {
      world->player.vel.y = std::max(world->player.vel.y + gravity / engine->ticktime.rate, min_yvel);
    }

    Rect<int> range = World::tilesFromAABB(world->player.getAABB());

    if (world->player.vel.y) {
      world->player.pos.y += world->player.vel.y / engine->ticktime.rate;
    }
    world->player.airborne = true;

    for (int x = range.x; x < range.x + range.w + 1; x++)
    for (int y = range.y; y < range.y + range.h + 1; y++) {
      Rect<float> plyrBox = world->player.getAABB();
      Rect<float> tileBox = World::tileAABB(x, y);

      if (x >= 0 and x < world->size.x
      and y >= 0 and y < world->size.y) {
        tileid_t tileid = world->getTile(x, y);
        if (tileid != 0
        and engine->getTileDef(tileid).type == TileType::SOLID
        and plyrBox.intersects(tileBox)) {
          Rect<float> collBox = plyrBox.intersection(tileBox);

          if (plyrBox.y + plyrBox.h / 2.f < tileBox.y + tileBox.h / 2.f) {
            if (world->player.pos.x > tileBox.x + tileBox.w
            and world->getTile(x + 1, y) == 1) {
              world->setTile(x + 1, y, 0);
            }
            else {
              world->setTile(x, y, 0);
            }
            engine->sound->play("brickshatter");
            if (world->player.vel.y > 0.f) {
              engine->sound->play("bump");
            }
            world->player.jumptime = 0.f;
            world->player.vel.y = 0.f;
            world->player.pos.y -= collBox.h;
          }
          else {
            world->player.airborne = false;
            world->player.vel.y = 0.f;
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
      Rect<float> plyrBox = world->player.getAABB();
      Rect<float> tileBox = World::tileAABB(x, y);

      if (x >= 0 and x < world->size.x
      and y >= 0 and y < world->size.y) {
        tileid_t tileid = world->getTile(x, y);
        if (tileid != 0
        and engine->getTileDef(tileid).type == TileType::SOLID
        and plyrBox.intersects(tileBox)) {
          Rect<float> collBox = plyrBox.intersection(tileBox);

          if (plyrBox.x + plyrBox.w / 2.f < tileBox.x + tileBox.w / 2.f) {
            world->player.vel.x = 0.f;
            world->player.pos.x -= collBox.w;
          }
          else {
            world->player.vel.x = 0.f;
            world->player.pos.x += collBox.w;
          }
        }
      }
    }

    world->camera.vel = (world->player.pos + Vec2(0.f, world->player.height / 2.f) - world->camera.pos) * engine->ticktime.rate / 8.f;
    world->camera.pos += world->camera.vel / engine->ticktime.rate;

    const Vec2f view_center(
      engine->viewport->getSize().x / 2.f / engine->viewscale,
      engine->viewport->getSize().y / 2.f / engine->viewscale
    );
    const Padding<float> camera_padding{
      .left = view_center.x + world->padding.left * 16.f,
      .right = world->size.x * 16.f - view_center.x - world->padding.right * 16.f,
      .bottom = view_center.y + world->padding.bottom * 16,
      .top = world->size.y * 16.f - view_center.y - world->padding.top * 16.f
    };

    if (world->camera.pos.x < camera_padding.left
    and world->camera.pos.x > camera_padding.right) {
      world->camera.pos.x = (camera_padding.left + camera_padding.right) / 2.f;
    }
    else {
      if (world->camera.pos.x < camera_padding.left) {
        world->camera.pos.x = camera_padding.left;
      }
      else if (world->camera.pos.x > camera_padding.right) {
        world->camera.pos.x = camera_padding.right;
      }
    }

    if (world->camera.pos.y > camera_padding.top
    and world->camera.pos.y < camera_padding.bottom) {
      world->camera.pos.y = (camera_padding.top + camera_padding.bottom) / 2.f;
    }
    else {
      if (world->camera.pos.y > camera_padding.top) {
        world->camera.vel.y = 0.f;
        world->camera.pos.y = camera_padding.top;
      }
      else if (world->camera.pos.y < camera_padding.bottom) {
        world->camera.vel.y = 0.f;
        world->camera.pos.y = camera_padding.bottom;
      }
    }
  }*/
}

void Gameplay::draw() {
  uint win_w = engine->viewport->getSize().x;
  uint win_h = engine->viewport->getSize().y;

  sf::View view(sf::Vector2f(), sf::Vector2f(win_w / engine->viewscale, win_h / engine->viewscale));
  view.setCenter(World::toView(world->camera->pos));
  engine->viewport->setView(view);

  drawBG(0x6898F8FF);
  drawBGBottom("overworldblockstop", Vec2(1.625f, 1.375f));
  drawBGTop("cloudlayer", Vec2(2.625f, 1.125f));
  drawTiles();
  drawEntities();
  drawUI();

  rendertime += engine->rendertime.delta;

  engine->viewport->display();
}

Gameplay::Gameplay(Engine* engine) : GameState(engine) {
  world = new World(engine, 176, 27);
}

Gameplay::~Gameplay() {
  if (world != nullptr) delete world;
}
}
