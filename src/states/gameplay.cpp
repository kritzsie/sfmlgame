#include "gameplay.hpp"

#include "pausemenu.hpp"
#include "../assetmanager.hpp"
#include "../engine.hpp"
#include "../entities.hpp"
#include "../tiletypes.hpp"

#include <SFML/System.hpp>

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

  float distdivx = dist.x / (dist.x - 1.f);
  float distdivy = dist.y / (dist.y - 1.f);

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

  float distdivx = dist.x / (dist.x - 1.f);
  float distdivy = dist.y / (dist.y - 1.f);

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

  float distdivx = dist.x / (dist.x - 1.f);
  float distdivy = dist.y / (dist.y - 1.f);

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

void Gameplay::drawUI() {
  const sf::Texture& sbfont = assets::gfx.getTile("smb3_sbfont");
  const Vec2u& viewsize = engine->viewsize;
  const std::string worldnum = "abcd0";
  const std::string p_meter = ">>>>>>()";
  const std::string coins = "$ 0";
  const std::string mario = "ABx 0";
  const std::string score = "0000000";
  const std::string timer = "@000";

  for (std::size_t i = 0; i < mario.length(); ++i) {
    const char& c = mario.at(i);
    const int x = c * 8 % sbfont.getSize().x;
    const int y = (c / 16) * 8 % sbfont.getSize().y;
    sf::Sprite sprite(sbfont, sf::IntRect(x, y, 8, 8));
    sprite.setPosition(8 * i + 18, 10);
    engine->viewport->draw(sprite);
  }

  for (std::size_t i = 0; i < p_meter.length(); ++i) {
    const char& c = p_meter.at(i);
    const int x = c * 8 % sbfont.getSize().x;
    const int y = (c / 16) * 8 % sbfont.getSize().y;
    sf::Sprite sprite(sbfont, sf::IntRect(x, y, 8, 8));
    sprite.setPosition(8 * i + 18, 18);
    engine->viewport->draw(sprite);
  }

  for (std::size_t i = 0; i < timer.length(); ++i) {
    const char& c = timer.at(i);
    const int x = c * 8 % sbfont.getSize().x;
    const int y = (c / 16) * 8 % sbfont.getSize().y;
    sf::Sprite sprite(sbfont, sf::IntRect(x, y, 8, 8));
    sprite.setPosition(8 * (i - timer.length() - 6) + viewsize.x - 2, 10);
    engine->viewport->draw(sprite);
  }

  for (std::size_t i = 0; i < coins.length(); ++i) {
    const char& c = coins.at(i);
    const int x = c * 8 % sbfont.getSize().x;
    const int y = (c / 16) * 8 % sbfont.getSize().y;
    sf::Sprite sprite(sbfont, sf::IntRect(x, y, 8, 8));
    sprite.setPosition(8 * (i - coins.length() - 2) + viewsize.x - 2, 10);
    engine->viewport->draw(sprite);
  }

  for (std::size_t i = 0; i < score.length(); ++i) {
    const char& c = score.at(i);
    const int x = c * 8 % sbfont.getSize().x;
    const int y = (c / 16) * 8 % sbfont.getSize().y;
    sf::Sprite sprite(sbfont, sf::IntRect(x, y, 8, 8));
    sprite.setPosition(8 * (i - score.length() - 2) + viewsize.x - 2, 18);
    engine->viewport->draw(sprite);
  }
}

void Gameplay::enter() {
  engine->music->change("overworld");
  engine->music->play();
  resume();
}

void Gameplay::exit() {
  engine->music->stop();
}

void Gameplay::pause() {
  paused = true;
}

void Gameplay::resume() {
  paused = false;
}

void Gameplay::update() {
  const Input& pause_input = engine->inputs[Action::pause];

  if (world->player->state & world->player->dead) {
    if (not paused) {
      engine->music->change("playerdown");
      pause();
    }
    world->player->die();
  }
  else if (not paused) {
    if (~pause_input > 0.f) {
      engine->sound->play("pause");
      engine->music->setVolume(0.5f);
      engine->pushState(PauseMenu::create(this));
      pause();
    }

    for (BaseEntity* entity : world->entities) {
      entity->update();
    }
  }
}

void Gameplay::draw() {
  const sf::Vector2u& viewsize = engine->viewport->getSize();

  sf::View view(
    World::toView(world->camera->pos),
    sf::Vector2f(viewsize.x / engine->viewscale, viewsize.y / engine->viewscale)
  );
  engine->viewport->setView(view);

  drawBG(0x6898F8FF);
  drawBGBottom("overworldblockstop", Vec2(1.625f, 1.375f));
  drawBGTop("cloudlayer", Vec2(2.625f, 1.125f));
  drawTiles();
  drawEntities();

  view = sf::View(sf::FloatRect(0.f, 0.f, engine->viewsize.x, engine->viewsize.y));
  engine->viewport->setView(view);

  drawUI();

  engine->viewport->display();

  rendertime += engine->rendertime.delta;
}

Gameplay::Gameplay(Engine* engine) : GameState(engine) {
  world = new World(engine, 176, 27);
}

Gameplay::~Gameplay() {
  if (world != nullptr) delete world;
}
}
