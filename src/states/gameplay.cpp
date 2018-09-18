#include "gameplay.hpp"

#include "../assetmanager.hpp"
#include "../engine.hpp"
#include "../entities.hpp"
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

void Gameplay::drawEntity(RenderEntity* renderentity) {
  const std::string& texture = renderentity->getFrame().texture;
  sf::Sprite sprite(assets::gfx.getSprite(texture));
  sprite.setPosition(World::toView(renderentity));
  sprite.setScale(sf::Vector2f(
    renderentity->scale.x * renderentity->getDirection(),
    renderentity->scale.y
  ));
  engine->viewport->draw(sprite);
}

void Gameplay::drawEntities() {
  for (BaseEntity* baseentity : world->entities) {
    if (baseentity == world->player) {
      continue;
    }

    RenderEntity* renderentity = dynamic_cast<RenderEntity*>(baseentity);
    if (renderentity != nullptr) {
      drawEntity(renderentity);
    }
  }

  drawEntity(world->player);
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
