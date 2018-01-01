#include "engine.hpp"
#include "entity.hpp"
#include "fmt/format.h"
#include "geometry.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <cstddef>
#include <cstdlib>

namespace engine {
// Static helper method used to flip the Y axis
template<typename T>
inline T World::toView(const T& vector) {
  return T(vector.x, -vector.y);
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
  player.pos = Vector2f(40, 32);
  camera.pos = player.pos;
  // End test world

  return true;
}

// TODO: Inherit from dedicated 2D vector class
inline tileid_t& World::getTile(const int x, const int y) {
  if (x < size.x)
    if (y < size.y)
      return tiles[x * size.y + y];
    else throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
  else throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

void World::setTile(const int x, const int y, const tileid_t tileid) {
  if (x < size.x)
    if (y < size.y)
      getTile(x, y) = tileid;
    else throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
  else throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

World::World(size_t x, size_t y) : player(Vector2f(7, 0), Vector2f(1, 1)) {
  tiles = new tileid_t[x * y]();
  size.x = x;
  size.y = y;
}

World::~World() {
  delete tiles;
}

bool Engine::init() {
  world->init();
  world->player.texture.loadFromFile("player.png");
  world->player.sprite.setTexture(world->player.texture);

  tick = 0;
  tickClock.restart();

  return true;
}

void Engine::resize(const size_t width, const size_t height) {
  window->setSize(Vector2u(width, height));
}

void Engine::onTick() {
  // Constants in tiles (16) per second
  static const float gravity = -32 * 16;
  static const float min_yvel = -16 * 16;

  if (keys.left ^ keys.right) {
    if (keys.left)
      world->player.vel.x = std::max(world->player.vel.x - 1.5f, keys.run ? -160.0f : -96.0f);
    else if (keys.right)
      world->player.vel.x = std::min(world->player.vel.x + 1.5f, keys.run ? 160.0f : 96.0f);
    world->player.facing = keys.left ? -1 : 1;
  }
  if (world->player.state.jumping) {
    if (world->player.vel.y > min_yvel) {
      world->player.vel.y = std::max(world->player.vel.y + gravity / tickRate, min_yvel);
    }
  }
  else if (!(keys.left ^ keys.right)) {
    if (world->player.vel.x > 0)
      world->player.vel.x = std::max(world->player.vel.x - 1, 0.0f);
    else if (world->player.vel.x < 0)
      world->player.vel.x = std::min(world->player.vel.x + 1, 0.0f);
  }
  world->player.vel += world->player.netForce / (tickRate * tickRate);
  world->player.netForce.x = 0;
  world->player.netForce.y = 0;
  world->player.pos += world->player.vel / tickRate;
  world->camera.pos += ((world->player.pos - world->camera.pos) * 4 - world->camera.vel / 2) / tickRate;

  tick++;
}

void Engine::render() {
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
      else {
        continue;
      }
    }
  }

  world->player.sprite.setPosition(world->player.toView());
  world->player.sprite.setScale(
    Vector2f(world->player.scale.x * world->player.facing,
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
      updateKeys();
  }
}

int Engine::exec() {
  init();

  float tickDelta = 0;
  while (window->isOpen()) {
    tickDelta = tickClock.getElapsedTime().asSeconds() - tickTime;
    tickTime = tickClock.getElapsedTime().asSeconds();
    if (int((tickTime - tickDelta) * tickRate) < int(tickTime * tickRate)) {
      onTick();
    }
    render();
  }

  return EXIT_SUCCESS;
}

void Engine::updateKeys() {
  keys.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
  keys.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
  keys.down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
  keys.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
  keys.jump = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
  keys.run = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
}

Engine::Engine(const arglist& args) : args(args) {
  window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Super Pixel Brawler");
  world = new World(128, 64);
  tickRate = 128;
  background.loadFromFile("background.png");
  tileart.loadFromFile("brick.png");
  updateKeys();
}

Engine::~Engine() {
  delete window;
  delete world;
}
}
