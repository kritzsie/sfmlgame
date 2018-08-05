#include "engine.hpp"
#include "assetmanager.hpp"
#include "geometry.hpp"
#include "types.hpp"
#include "world.hpp"

#include <physfs.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <stdexcept>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctgmath>

namespace ke {
std::size_t Engine::instance_count = 0;

bool Engine::Keys::State::getState() const {
  return state;
}

byte Engine::Keys::State::getDelta() const {
  return delta;
}

void Engine::Keys::State::setState(bool pressed) {
  delta = (pressed ? 1 : 0) - state;
  state = pressed ? true : false;
}

void Engine::Keys::State::press() {
  setState(true);
}

void Engine::Keys::State::release() {
  setState(false);
}

void Engine::Keys::State::resetDelta() {
  delta = 0;
}

void Engine::resize(Vec2<std::size_t> size) {
  auto width = size.x;
  auto height = size.y;
  window->setSize(sf::Vector2u(width, height));
}

void Engine::onKeyEvent() {
  switch (event.key.code) {
  case sf::Keyboard::Up:
    keys.up.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
    break;
  case sf::Keyboard::Left:
    keys.left.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
    break;
  case sf::Keyboard::Down:
    keys.down.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
    break;
  case sf::Keyboard::Right:
    keys.right.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
    break;
  case sf::Keyboard::X:
    keys.jump.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::X));
    break;
  case sf::Keyboard::Z:
    keys.run.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
  default:
    break;
  }
}

void Engine::tickKeys() {
  keys.up.resetDelta();
  keys.left.resetDelta();
  keys.down.resetDelta();
  keys.right.resetDelta();
  keys.jump.resetDelta();
  keys.run.resetDelta();
}

void Engine::doTick() {
  // TODO: Send these to a better place
  const float gravity = -48 * 16;
  const float min_yvel = -16 * 16;

  // Handle keyboard input first
  auto up = keys.up.getState();
  auto left = keys.left.getState();
  auto down = keys.down.getState();
  auto right = keys.right.getState();
  auto jump = keys.jump.getState();
  auto run = keys.run.getState();

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
        world->player.vel.x = std::max(-8.0f * 16.0f, std::min(world->player.vel.x + direction * 16 * 16 / tickRate, max_xvel));
      }
      else if (direction < 0
      and world->player.vel.x > min_xvel) {
        world->player.vel.x = std::max(min_xvel, std::min(world->player.vel.x + direction * 16 * 16 / tickRate, 8.0f * 16.0f));
      }
    }
    else if (world->player.vel.x > 0) {
      world->player.vel.x = std::max(world->player.vel.x - 12 * 16 / tickRate, 0.0f);
    }
    else if (world->player.vel.x < 0) {
      world->player.vel.x = std::min(world->player.vel.x + 12 * 16 / tickRate, 0.0f);
    }

    if (down and not direction) {
      world->player.sprite.setTexture(gfxassets["bigmarioduck"], true);
      world->player.offset.x = 7;
      world->player.duck();
    }
    else {
      if (world->player.vel.x > 0
      and direction < 0) {
        world->player.sprite.setTexture(gfxassets["bigmarioslip"], true);
        world->player.offset.x = 8;
        if (world->player.sliptime == 0) {
          world->player.sliptime = 0.09375;
          sound->play("slip");
        }
        else {
          world->player.sliptime = std::max(0.0f, world->player.sliptime - 1 / tickRate);
        }
      }
      else if (world->player.vel.x < 0
      and direction > 0) {
        world->player.sprite.setTexture(gfxassets["bigmarioslip"], true);
        world->player.offset.x = 8;
        if (world->player.sliptime == 0) {
          world->player.sliptime = 0.09375;
          sound->play("slip");
        }
        else {
          world->player.sliptime = std::max(0.0f, world->player.sliptime - 1 / tickRate);
        }
      }
      else {
        if (world->player.vel.x) {
          if (world->player.walktime) {
            world->player.walktime = std::max(0.0f, world->player.walktime - std::max(1.0f, std::abs(world->player.vel.x) / 32) / tickRate);
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
        world->player.sprite.setTexture(gfxassets["bigmariowalk_" + std::to_string(world->player.walkcycle < 3 ? world->player.walkcycle : 1)], true);
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
        world->player.vel.x = std::min(world->player.vel.x + direction * 10 * 16 / tickRate, max_xvel);
      }
      else if (direction < 0
      and world->player.vel.x > min_xvel) {
        world->player.vel.x = std::max(min_xvel, world->player.vel.x + direction * 10 * 16 / tickRate);
      }
    }
    if (not world->player.ducking) {
      world->player.sprite.setTexture(gfxassets["bigmariojump"], true);
      world->player.offset.x = 8;
    }
  }

  if (not world->player.underwater) {
    if (keys.jump.getDelta() == 1) {
      if (not world->player.airborne) {
        world->player.jumptime = 0.3125;
        sound->play("jump");
      }
    }
    else if (keys.jump.getDelta() == -1) {
      world->player.jumptime = 0;
    }

    if (jump and world->player.jumptime > 0) {
      world->player.jumptime = std::max(world->player.jumptime - 1 / tickRate, 0.0f);
      world->player.jump();
    }
  }

  if (world->player.vel.y > min_yvel
  and world->player.jumptime == 0) {
    world->player.vel.y = std::max(world->player.vel.y + gravity / tickRate, min_yvel);
  }

  auto range = World::tilesFromAABB(world->player.getAABB());

  if (world->player.vel.y) {
    world->player.pos.y += world->player.vel.y / tickRate;
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
          sound->play("brickshatter");*/
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
    world->player.pos.x += world->player.vel.x / tickRate;
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

        if (false) {
          world->player.pos.x += 1;
        }
      }
    }
  }

  world->camera.vel = (world->player.pos + Vec2f(0, world->player.height / 2) - world->camera.pos) * tickRate / 8;

  world->camera.pos += world->camera.vel / tickRate;

  auto camLeft = window->getSize().x / 6;
  auto camRight = world->size.x * 16 - window->getSize().x / 6;
  auto camUp = world->size.y * 16 - window->getSize().y / 6;
  auto camDown = window->getSize().y / 6;

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

  tickKeys();
  tick++;
}

void Engine::drawBG(uint32_t color) {
  window->clear(sf::Color(color));
}

void Engine::drawBG(std::string bg, Vec2f dist) {
  auto win_w = window->getSize().x;
  auto win_h = window->getSize().y;

  const auto& texture = gfxassets[bg];

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera.pos.x - win_w / 6;
  float bottom = world->camera.pos.y - win_h / 6;
  int sky_w = texture.getSize().x;
  int sky_h = texture.getSize().y;
  int min_x = left / sky_w;
  int min_y = bottom / sky_h;
  int max_x = (world->camera.pos.x + win_w * (2 * dist.x - 1) / 6) / sky_w + dist.x;
  int max_y = (world->camera.pos.y + win_h * (2 * dist.y - 1) / 6) / sky_h + dist.y;

  sf::Sprite sky(texture);
  for (int y = floor(min_y / dist.y); y < floor(max_y / dist.y) + 1; y++)
  for (int x = floor(min_x / dist.x); x < floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2f(x * sky_w + left / distdivx, y * sky_h + sky_h + bottom / distdivy)));
    window->draw(sky);
  }
}

void Engine::drawBGBottom(std::string bg, Vec2f dist) {
  auto win_w = window->getSize().x;
  auto win_h = window->getSize().y;

  const auto& texture = gfxassets[bg];

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera.pos.x - win_w / 6;
  float bottom = world->camera.pos.y - win_h / 6;
  int sky_w = texture.getSize().x;
  int sky_h = texture.getSize().y;
  int min_x = left / sky_w;
  int max_x = (world->camera.pos.x + win_w * (2 * dist.x - 1) / 6) / sky_w + dist.x;

  sf::Sprite sky(texture);
  for (int x = floor(min_x / dist.x); x < floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2f(x * sky_w + left / distdivx, sky_h + bottom / distdivy)));
    window->draw(sky);
  }
}

void Engine::drawBGTop(std::string bg, Vec2f dist) {
  auto win_w = window->getSize().x;
  auto win_h = window->getSize().y;

  const auto& texture = gfxassets[bg];

  float distdivx = dist.x / (dist.x - 1.0f);
  float distdivy = dist.y / (dist.y - 1.0f);

  float left = world->camera.pos.x - win_w / 6;
  float top = world->camera.pos.y + win_h / 6;
  int sky_w = texture.getSize().x;
  int min_x = left / sky_w;
  int max_x = (world->camera.pos.x + win_w * (2 * dist.x - 1) / 6) / sky_w + dist.x;

  sf::Sprite sky(texture);
  for (int x = floor(min_x / dist.x); x < floor(max_x / dist.x) + 1; x++) {
    sky.setPosition(World::toView(Vec2f(x * sky_w + left / distdivx, world->size.y * 16 / dist.y + top / distdivy)));
    window->draw(sky);
  }
}

void Engine::drawTiles() {
  auto win_w = window->getSize().x;
  auto win_h = window->getSize().y;

  int left = std::max(0, std::min(int(floor((world->camera.pos.x - win_w / 6) / 16)), world->size.x));
  int bottom = std::max(0, std::min(int(floor((world->camera.pos.y - win_h / 6) / 16)), world->size.y));
  int right = std::max(0, std::min(int(floor((world->camera.pos.x + win_w / 6) / 16)) + 1, world->size.x));
  int top = std::max(0, std::min(int(floor((world->camera.pos.y + win_h / 6) / 16)) + 1, world->size.y));

  for (int y = bottom; y < top; y++)
  for (int x = left; x < right; x++) {
    auto tileid = world->getTile(x, y);
    if (tileid) {
      const sf::Texture& texture = gfxassets["smb3_atlas"];
      int xoffset = (tileid - 1) * 16 % texture.getSize().x;
      int yoffset = ((tileid - 1) / (texture.getSize().x / 16)) * 16 % texture.getSize().y;
      sf::Sprite tile(texture, sf::IntRect(xoffset, yoffset, 16, 16));
      tile.setPosition(World::toView(Vec2f(x * 16, y * 16 + 16)));
      window->draw(tile);
    }
  }
}

void Engine::drawEntities() {
  world->player.sprite.setPosition(world->player.toView());
  world->player.sprite.setScale(Vec2f(
    world->player.scale.x * world->player.getDirection(),
    world->player.scale.y
  ));
  window->draw(world->player.sprite);
}

void Engine::drawUI() {
}

void Engine::doRender() {
  auto win_w = window->getSize().x;
  auto win_h = window->getSize().y;

  sf::View view(Vec2f(), Vec2f(floor(win_w / 3), floor(win_h / 3)));
  view.setCenter(World::toView(world->camera.pos));
  window->setView(view);

  drawBG(0x6898F8FF);
  drawBGBottom("overworldblockstop", Vec2f(1.625, 1.375));
  drawBGTop("cloudlayer", Vec2f(2.625, 1.125));
  drawTiles();
  drawEntities();
  drawUI();

  window->display();
}

bool Engine::init() {
  PhysFS::init(args.at(0).c_str());

  window->create(sf::VideoMode(768, 576), "Super Mario Bros. 3");

  if (not world->init()) {
    return false;
  }
  if (not sound->init()) {
    return false;
  }
  if (not music->init()) {
    return false;
  }

  tick = 0;
  tickClock.restart();

  music->change("overworld");
  music->play();

  world->player.sprite.setTexture(gfxassets["bigmariowalk_0"]);

  return true;
}

int Engine::exec() {
  if (not init()) {
    return EXIT_FAILURE;
  }

  float tickDelta = 0;
  while (window->isOpen()) {
    tickDelta = tickClock.getElapsedTime().asSeconds() - tickTime;
    tickTime = tickClock.getElapsedTime().asSeconds();

    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window->close();
      }
      else if (event.type == sf::Event::Resized) {
        resize(Vec2<std::size_t>(event.size.width, event.size.height));
      }
      else if (event.type == sf::Event::KeyPressed
           or  event.type == sf::Event::KeyReleased) {
        onKeyEvent();
      }
    }

    if (int((tickTime - tickDelta) * tickRate) < int(tickTime * tickRate)) {
      doTick();
    }

    doRender();
  }

  return EXIT_SUCCESS;
}

bool Engine::setupPhysFS() {
  PHYSFS_permitSymbolicLinks(true);
  if (PHYSFS_setSaneConfig("Kha0z", "smb3", nullptr, 0, 0) == 0) {
    return false;
  }
  if (PHYSFS_mount("basesmb3", "", 1) == 0) {
    return false;
  }
  return true;
}

Engine::Engine(const arglist& args) : args(args), tickRate(64) {
  if (instance_count == 0) {
    PHYSFS_init(args.at(0).c_str());
  }
  instance_count++;

  setupPhysFS();

  window = new sf::RenderWindow;
  world = new World(176, 27);
  music = new Music;
  sound = new Sound;
}

Engine::~Engine() {
  if (instance_count > 0) {
    if (instance_count == 1) {
      PHYSFS_deinit();
    }
    instance_count--;
  }

  if (window != nullptr) delete window;
  if (world  != nullptr) delete world;
  if (music  != nullptr) delete music;
  if (sound  != nullptr) delete sound;
}
}
