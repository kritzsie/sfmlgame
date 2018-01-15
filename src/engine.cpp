#include "engine.hpp"
#include "entity.hpp"
#include "fmt/format.h"
#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <cstddef>
#include <cstdlib>
#include <ctgmath>

namespace engine {
// Static helper method used to flip the Y axis
template<typename T>
T World::toView(const T& vector) {
  return T(vector.x, -vector.y);
}

template<typename T>
Rect<int> World::tilesFromAABB(const Rect<T>& bbox) {
  return Rect<int>(
    floor(bbox.x / 16) - 1, floor(bbox.y / 16) - 1,
    ceil(bbox.w / 16) + 2, ceil(bbox.h / 16) + 2
  );
}

Rect<float> World::tileAABB(int x, int y) {
  return Rect<float>(x * 16, y * 16, 16, 16);
}

// TODO: Inherit from dedicated 2D vector class
tileid_t& World::getTile(int x, int y) {
  if (x < size.x)
    if (y < size.y)
      return tiles[x * size.y + y];
    else throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
  else throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

void World::setTile(int x, int y, tileid_t tileid) {
  if (x < size.x)
    if (y < size.y)
      getTile(x, y) = tileid;
    else throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
  else throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

bool World::init() {
  // WARNING: Test world ahead
  for (int x = 0; x < 8; x++) for (int y = 0; y < 2; y++) {
      setTile(x, y, 1);
  }
  setTile(0, 2, 1);
  setTile(5, 2, 1);
  setTile(6, 2, 1);
  setTile(6, 3, 1);
  setTile(7, 2, 1);
  setTile(7, 3, 1);
  setTile(7, 4, 1);
  for (int x = 12; x < 24; x++) for (int y = 0; y < 2; y++) {
    setTile(x, y, 1);
  }
  setTile(23, 2, 1);
  player.pos = Vector2f(40, 32);
  camera.pos = player.pos + Vector2f(0, player.height / 2);
  // End test world

  return true;
}

World::World(size_t x, size_t y) : player({7, 0}, {1, 1}, 5, 25) {
  tiles = new tileid_t[x * y]();
  size.x = x;
  size.y = y;
}

World::~World() {
  delete tiles;
}

void Engine::Keys::State::set(bool pressed) {
  delta = (pressed ? 1 : 0) - state;
  state = pressed ? true : false;
}

void Engine::Keys::State::press() {
  set(true);
}

void Engine::Keys::State::release() {
  set(false);
}

bool Engine::Keys::State::get() const {
  return state;
}

byte Engine::Keys::State::popDelta() {
  auto retval = delta;
  delta = 0;
  return retval;
}

bool Engine::init() {
  world->init();
  world->player.texture.loadFromFile("player.png");
  world->player.sprite.setTexture(world->player.texture);

  tick = 0;
  tickClock.restart();

  return true;
}

void Engine::resize(size_t width, size_t height) {
  window->setSize(sf::Vector2u(width, height));
}

void Engine::doTick() {
  // TODO: Send these to a better place
  static const float gravity = -32 * 16;
  static const float min_yvel = -16 * 16;

  if (keys.left != keys.right) {
    if (keys.left)
      world->player.vel.x = std::max(world->player.vel.x - 1.5f, keys.run ? -160.f : -96.f);
    else if (keys.right)
      world->player.vel.x = std::min(world->player.vel.x + 1.5f, keys.run ? 160.f : 96.f);
    world->player.setDirection(keys.left ? -1 : 1);
  }

  if (keys.jump) {
    world->player.vel.y = 8 * 16;
  }

  if (world->player.airborne) {
    if (world->player.vel.y > min_yvel) {
      world->player.vel.y = std::max(world->player.vel.y + gravity / tickRate, min_yvel);
    }
  }
  else if (keys.left == keys.right) {
    if (world->player.vel.x > 0) {
      world->player.vel.x = std::max(world->player.vel.x - 1.f, 0.f);
    }
    else if (world->player.vel.x < 0) {
      world->player.vel.x = std::min(world->player.vel.x + 1.f, 0.f);
    }
  }

  world->player.pos.x += world->player.vel.x / tickRate;

  auto range = World::tilesFromAABB(world->player.getAABB());
  for (int y = range.y; y < range.y + range.h; y++)
  for (int x = range.x; x < range.x + range.w; x++) {
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

  world->player.pos.y += world->player.vel.y / tickRate;
  world->player.airborne = true;

  for (int y = range.y; y < range.y + range.h; y++)
  for (int x = range.x; x < range.x + range.w; x++) {
    auto plyrBox = world->player.getAABB();
    auto tileBox = World::tileAABB(x, y);

    if (x >= 0 and x < world->size.x
    and y >= 0 and y < world->size.y) {
      if (world->getTile(x, y) != 0
      and plyrBox.intersects(tileBox)) {
        auto collBox = plyrBox.intersection(tileBox);

        if (plyrBox.y + plyrBox.h / 2 < tileBox.y + tileBox.h / 2) {
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

  world->camera.vel = (world->player.pos + Vector2f(0, world->player.height / 2) - world->camera.pos) * tickRate / 16;

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
      world->camera.pos.y = camDown + 0.5;
    }
  }

  tick++;
}

void Engine::doRender() {
  auto width = window->getSize().x;
  auto height = window->getSize().y;

  sf::View view(Vector2f(), Vector2u(width, height) / 3);
  view.setCenter(World::toView(world->camera.pos));
  window->setView(view);

  sf::Sprite sky(background);
  sky.setPosition(World::toView(world->camera.pos - World::toView(Vector2f(background.getSize()) / 2)));
  window->draw(sky);

  sf::Sprite brick(tileart);
  for (int y = 0; y < world->size.y; y++) {
    for (int x = 0; x < world->size.x; x++) {
      if (world->getTile(x, y)) {
        brick.setPosition(World::toView(Vector2f(x * 16, y * 16 + 16)));
        window->draw(brick);
      }
    }
  }

  world->player.sprite.setPosition(world->player.toView());
  world->player.sprite.setScale(
    Vector2f(world->player.scale.x * world->player.getDirection(),
             world->player.scale.y));
  window->draw(world->player.sprite);

  window->display();

  while (window->pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window->close();
    else if (event.type == sf::Event::Resized)
      resize(event.size.width, event.size.height);
    else if (event.type == sf::Event::KeyPressed
         or  event.type == sf::Event::KeyReleased)
      onKeyEvent();
  }
}

int Engine::exec() {
  init();

  float tickDelta = 0;
  while (window->isOpen()) {
    tickDelta = tickClock.getElapsedTime().asSeconds() - tickTime;
    tickTime = tickClock.getElapsedTime().asSeconds();
    if (int((tickTime - tickDelta) * tickRate) < int(tickTime * tickRate)) {
      doTick();
    }
    doRender();
  }

  return EXIT_SUCCESS;
}

void Engine::onKeyEvent() {
  switch (event.key.code) {
  case sf::Keyboard::Up:
    keys.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    break;
  case sf::Keyboard::Left:
    keys.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    break;
  case sf::Keyboard::Down:
    keys.down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    break;
  case sf::Keyboard::Right:
    keys.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    break;
  case sf::Keyboard::X:
    keys.jump = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    break;
  case sf::Keyboard::Z:
    keys.run = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
  }
}

Engine::Engine(const arglist& args) : args(args), keys{false} {
  window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Super Pixel Brawler");
  world = new World(128, 32);
  tickRate = 128;
  background.loadFromFile("background.png");
  tileart.loadFromFile("brick.png");
}

Engine::~Engine() {
  delete window;
  delete world;
}
}
