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
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 6; x++) {
      setTile(x, y, 1);
    }
  }
  setTile(0, 2, 1);
  setTile(size.x - 1, size.y - 1, 1);
  state.player.pos = Vector2f(40, 32);
  state.camera.pos = state.player.pos;
  // End test world

  prev = state;

  return true;
}

World::World(size_t x, size_t y) {
  tiles = new tileid_t[x * y]();
  size.x = x;
  size.y = y;

  state.player = PlayerEntity({7, 0}, {1, 1}, 5, 24);
}

World::~World() {
  delete tiles;
}

bool Engine::init() {
  world->init();
  world->state.player.texture.loadFromFile("player.png");
  world->state.player.sprite.setTexture(world->state.player.texture);

  tick = 0;
  tickClock.restart();

  return true;
}

void Engine::resize(size_t width, size_t height) {
  window->setSize(sf::Vector2u(width, height));
}

void Engine::doTick() {
  // Constants in tiles (16) per second
  static const float gravity = -32 * 16;
  static const float min_yvel = -16 * 16;

  world->prev = world->state;

  if (keys.left != keys.right) {
    if (keys.left)
      world->player.vel.x = std::max(world->player.vel.x - 1.5f, keys.run ? -160.f : -96.f);
    else if (keys.right)
      world->player.vel.x = std::min(world->player.vel.x + 1.5f, keys.run ? 160.f : 96.f);
    world->player.facing = keys.left ? -1 : 1;
  }

  if (keys.jump) {
    world->player.airborne = true;
    world->player.vel.y = 8 * 16;
  }

  if (world->player.airborne) {
    if (world->player.vel.y > min_yvel) {
      world->player.vel.y = std::max(world->player.vel.y + gravity / tickRate, min_yvel);
    }
  }
  else {
    if (keys.left == keys.right) {
      if (world->player.vel.x > 0) {
        world->player.vel.x = std::max(world->player.vel.x - 1, 0.0f);
      }
      else if (world->player.vel.x < 0) {
        world->player.vel.x = std::min(world->player.vel.x + 1, 0.0f);
      }
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

        if (plyrBox.x + plyrBox.w / 2 > tileBox.x + tileBox.w / 2) {
          world->player.vel.x = 0;
          world->player.pos.x += collBox.w;
        }
        else /*if (plyrBox.x + plyrBox.w / 2 < tileBox.x + tileBox.w / 2)*/ {
          world->player.vel.x = 0;
          world->player.pos.x -= collBox.w;
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

        if (plyrBox.y + plyrBox.h / 2 > tileBox.y + tileBox.h / 2) {
          world->player.airborne = false;
          world->player.vel.y = 0;
          world->player.pos.y += collBox.h;
        }
        else /*if (plyrBox.y + plyrBox.h / 2 < tileBox.y + tileBox.h / 2)*/ {
          world->player.vel.y = 0;
          world->player.pos.y -= collBox.h;
        }
      }
    }
  }

  world->state.camera.pos += ((world->state.player.pos - world->state.camera.pos) * 4 - world->state.camera.vel / 2) / tickRate;

  tick++;
}

void Engine::doRender() {
  auto width = window->getSize().x;
  auto height = window->getSize().y;

  sf::View view(Vector2f(), Vector2u(width, height) / 3);
  view.setCenter(World::toView(world->state.camera.pos));
  window->setView(view);

  sf::Sprite sky(background);
  sky.setPosition(World::toView(world->state.camera.pos - World::toView(Vector2f(background.getSize()) / 2)));
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

  // FIXME: Set correct position for scaled sprites
  world->state.player.sprite.setPosition(world->state.player.toView());
  world->state.player.sprite.setScale(
    Vector2f(world->state.player.scale.x * world->state.player.facing,
             world->state.player.scale.y));
  window->draw(world->state.player.sprite);

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
  world = new World(16, 8);
  tickRate = 128;
  background.loadFromFile("background.png");
  tileart.loadFromFile("brick.png");
}

Engine::~Engine() {
  delete window;
  delete world;
}
}
