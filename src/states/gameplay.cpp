#include "basestate.hpp"

#include "basegame.hpp"
#include "pausemenu.hpp"
#include "../assetmanager.hpp"
#include "../engine.hpp"
#include "../entities.hpp"
#include "../tiletypes.hpp"

#include <SFML/System.hpp>

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <string>
#include <optional>

namespace ke {
static char highASCII(char c) {
  return c |= 128;
}

static std::string highASCII(std::string str) {
  for (auto& c : str) {
    c = highASCII(c);
  }
  return str;
}

TextStyle::TextStyle(std::string font, bool align_right, bool align_bottom)
: font(font), align(align_right, align_bottom) {}

TextStyle::TextStyle(bool align_right, bool align_bottom)
: TextStyle("smb3_sbfont", align_right, align_bottom) {}

TextStyle::TextStyle(std::string font) : TextStyle(font, false, false) {}

Gameplay::Factory Gameplay::create(BaseGame* basegame) {
  return [=](Engine* engine) -> BaseState* {
    return new Gameplay(engine, basegame);
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
    sky.setPosition(World::toView(Vec2(
      x * sky_w + left / distdivx + 16.f * world->padding.left / dist.x,
      world->getSize().y * 16.f / dist.y + top / distdivy - 16.f * world->padding.top / dist.y)
    ));
    engine->viewport->draw(sky);
  }
}

void Gameplay::drawTiles() {
  uint win_w = engine->viewport->getSize().x;
  uint win_h = engine->viewport->getSize().y;

  int left = std::max(0, std::min(int((world->camera->pos.x - win_w / 2 / engine->viewscale) / 16), world->getSize().x));
  int bottom = std::max(0, std::min(int((world->camera->pos.y - win_h / 2 / engine->viewscale) / 16), world->getSize().y));
  int right = std::max(0, std::min(int((world->camera->pos.x + win_w / 2 / engine->viewscale) / 16) + 1, world->getSize().x));
  int top = std::max(0, std::min(int((world->camera->pos.y + win_h / 2 / engine->viewscale) / 16) + 1, world->getSize().y));

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
  for (BaseEntity* baseentity : world->getEntities()) {
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

void Gameplay::drawText(std::string text, Vec2f pos, TextStyle style) {
  const sf::Texture& texture = assets::gfx.getTexture(style.font);
  const Vec2u viewsize = engine->viewsize;
  const std::size_t length = text.length();

  for (std::size_t i = 0; i < length; ++i) {
    const ubyte c = text.at(i);
    const sf::IntRect cliprect(
      c * 8 % texture.getSize().x, (c / 16) * 8 % texture.getSize().y, 8, 8
    );
    sf::Sprite sprite(texture, cliprect);
    float x_pos = style.align.x ? viewsize.x - 8.f * length - pos.x : pos.x;
    float y_pos = style.align.y ? viewsize.y - 8.f - pos.y : pos.y;
    sprite.setPosition(x_pos + 8.f * i, y_pos);
    engine->viewport->draw(sprite);
  }
}

void Gameplay::drawText(std::string text, Vec2f pos) {
  drawText(text, pos, TextStyle());
}

void Gameplay::drawUI() {
  std::stringstream worldnum;
  worldnum << highASCII("abcd") << '\0' << '-' << 1;

  std::stringstream p_meter;
  for (std::size_t i = 0; i < 6; ++i) {
    p_meter << (world->player->p_meter >= (i + 1) ? highASCII('>') : '>');
  }
  if (world->player->p_meter >= 7.f
  and std::fmod(rendertime, 0.25f) > 0.125f) {
    p_meter << highASCII("()");
  }
  else {
    p_meter << "()";
  }

  std::stringstream coins;
  coins << "$" << std::setw(2) << basegame->getCoins();

  std::stringstream mario;
  mario << highASCII("ABx") << std::setw(2) << basegame->getLives();

  std::stringstream score;
  score << std::internal << std::setw(7) << std::setfill('0') << basegame->getScore();

  std::stringstream timerstr;
  timerstr << "@" << std::fixed << std::internal
           << std::setprecision(0) << std::setw(3) << std::setfill('0')
           << int(std::ceil(world->getTimer()));

  //TextStyle align_left(false, false);
  drawText(worldnum.str(), Vec2f(16, 16));
  drawText(mario.str(), Vec2f(16, 24));

  TextStyle align_right(true, false);
  drawText(timerstr.str(), Vec2f(48, 16), align_right);
  drawText(coins.str(), Vec2f(16, 16), align_right);
  drawText(score.str(), Vec2f(16, 24), align_right);

  TextStyle align_bottom(false, true);
  drawText(p_meter.str(), Vec2f(64, 6), align_bottom);
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
  if (world->player->state & world->player->dead) {
    if (not paused) {
      if (world->player->snd_running.has_value()) {
        engine->sound->stop(world->player->snd_running.value());
        world->player->snd_running = std::nullopt;
      }
      engine->music->change("playerdown");
      pause();
    }
    world->player->die();
  }
  else if (not paused) {
    const Input& pause_input = engine->inputs[Action::pause];

    if (~pause_input > 0.f) {
      if (world->player->snd_running.has_value()) {
        engine->sound->stop(world->player->snd_running.value());
        world->player->snd_running = std::nullopt;
      }
      engine->sound->play("pause");
      engine->music->setVolume(0.5f);
      engine->pushState(PauseMenu::create(this));
      pause();
    }
    else {
      world->update();
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

Gameplay::Gameplay(Engine* engine, BaseGame* basegame)
: BaseState(engine), basegame(basegame) {
  world = new World(engine, basegame, 176, 27);
}

Gameplay::~Gameplay() {
  if (world != nullptr) delete world;
}
}
