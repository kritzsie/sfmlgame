#include "engine.hpp"
#include "fmt/format.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <cstddef>
#include <cstdlib>

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace engine {
  BaseEntity::BaseEntity() {}

  void Entity::applyForce(const sf::Vector2f& f) {
    netForce += f;
  }

  void Entity::applyForce(const float x, const float y) {
    applyForce(sf::Vector2f(x, y));
  }

  Entity::Entity(const float mass) : BaseEntity() {
    this->mass = mass;
  }

  bool World::init() {
    for (unsigned int y = 0; y < 2; y++) {
      for (unsigned int x = 0; x < 6; x++) {
        setTile(x, y, 1);
      }
    }
    setTile(0, 2, 1);
    setTile(size.x - 1, size.y - 1, 1);
    player.pos = sf::Vector2f(40, 32);
    player.vel += sf::Vector2f(2, 16) * 16.0f;
    camera.pos = player.pos;
    return true;
  }

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

  World::World(size_t x, size_t y) : player(80) {
    tiles = new tileid_t[x * y]();
    size.x = x;
    size.y = y;
  }

  World::~World() {
    delete tiles;
  }

  bool Engine::init() {
    auto width  = window->getSize().x;
    auto height = window->getSize().y;

    world->init();
    world->player.texture.loadFromFile("player.png");
    world->player.sprite.setTexture(world->player.texture);

    tickClock.restart();

    return true;
  }

  void Engine::resize(const size_t width, const size_t height) {
    window->setSize(sf::Vector2u(width, height));
  }

  void Engine::tick() {
    // Constants in tiles (16) per second
    static const float gravity = -16 * 16;
    static const float min_yvel = -16 * 16;

    world->player.vel += world->player.netForce / (tickRate * tickRate);
    if (world->player.vel.y > min_yvel)
      world->player.vel.y = fmax(world->player.vel.y + gravity / tickRate, min_yvel);
    world->player.pos += world->player.vel / tickRate;
    world->camera.pos += (4.0f * (world->player.pos - world->camera.pos) - world->camera.vel / 2.0f) / tickRate;
  }

  void Engine::render() {
    auto width  = window->getSize().x;
    auto height = window->getSize().y;

    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window->close();
      else if (event.type == sf::Event::Resized)
        resize(event.size.width, event.size.height);
    }

    auto view = sf::View(sf::Vector2f(), sf::Vector2f(width, height) / 3.0f);
    view.setCenter(sf::Vector2f(world->camera.pos.x, -world->camera.pos.y));
    window->setView(view);

    auto tile = sf::Sprite(tileart);
    for (int y = 0; y < world->size.y; y++) {
      for (int x = 0; x < world->size.x; x++) {
        if (world->getTile(x, y)) {
          tile.setPosition(x * 16, -y * 16);
          window->draw(tile);
        }
        else {
          continue;
        }
      }
    }
    world->player.sprite.setPosition(sf::Vector2f(world->player.pos.x, -world->player.pos.y));
    window->draw(world->player.sprite);

    window->display();
  }

  int Engine::run() {
    init();

    float tickDelta;
    while (window->isOpen()) {
      tickDelta = tickClock.getElapsedTime().asSeconds() - tickTime;
      tickTime = tickClock.getElapsedTime().asSeconds();
      if (int((tickTime - tickDelta) * tickRate) < int(tickTime * tickRate)) {
        tick();
      }
      render();
    }

    return EXIT_SUCCESS;
  }

  Engine::Engine(const arglist& args) : args(args) {
    window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Super Pixel Brawler");
    world = new World(128, 64);
    tickRate = 128;
    tileart.loadFromFile("brick.png");
  }

  Engine::~Engine() {
    delete window;
    delete world;
  }
}
