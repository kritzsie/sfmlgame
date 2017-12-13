#include <iostream>
#include <sstream>
#include <stdexcept>

#include <cmath>
#include <cstddef>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "engine.hpp"

namespace engine {
  BaseEntity::BaseEntity(const entid_t entid) {}

  void Entity::applyForce(const sf::Vector2f& f) {
    netForce += f;
  }

  void Entity::applyForce(const float x, const float y) {
    applyForce(sf::Vector2f(x, y));
  }

  Entity::Entity(const entid_t entid, const float mass) : BaseEntity(entid), mass(mass) {}

  bool World::init() {
    for (int x = 0; x < 4; x++) {
      for (int y = 6; y < 8; y++) {
        setTile(x, y, 1);
      }
    }
    setTile(0, 2, 1);
    setTile(size.x - 1, size.y - 1, 1);
    return true;
  }

  tileid_t& World::getTile(const int x, const int y) {
    if (x < size.x and y < size.y) {
      return tiles[x * size.y + y];
    }
    else {
      throw std::out_of_range([x, y, this]() {
        std::stringstream what;
        if (x < size.x)
          what << "x coordinate out of bounds";
        else if (y < size.y)
          what << "y coordinate out of bounds";
        else
          what << "x, y coordinates out of bounds";
        return what.str();
      }());
    }
  }

  void World::setTile(const int x, const int y, const tileid_t tileid) {
    if (x < size.x and y < size.y) {
      tiles[x * size.y + y] = tileid;
    }
    else {
      throw std::out_of_range([x, y, this]() {
        std::stringstream what;
        if (x < size.x)
          what << "x coordinate out of bounds";
        else if (y < size.y)
          what << "y coordinate out of bounds";
        else
          what << "x, y coordinates out of bounds";
        return what.str();
      }());
    }
  }

  World::World(size_t x, size_t y) : player(0, 80), camera(0) {
    tiles = new tileid_t[x * y]();
    size.x = x;
    size.y = y;
    player.pos = sf::Vector2f(24, 128);
    player.vel += sf::Vector2f(2, -16) * 16.0f;
    camera.pos = player.pos;
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
    static const float gravity = 16 * 16;
    static const float max_yvel = 16 * 16;

    world->player.vel += world->player.netForce / (tickRate * tickRate);
    if (world->player.vel.y < max_yvel)
      world->player.vel.y = fmin(world->player.vel.y + gravity / tickRate, max_yvel);
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
    view.setCenter(world->camera.pos);
    window->setView(view);

    auto tile = sf::Sprite(tileart);
    for (int y = 0; y < world->size.y; y++) {
      for (int x = 0; x < world->size.x; x++) {
        if (world->getTile(x, y)) {
          tile.setTextureRect(sf::IntRect(17 * 15, 17 * 8, 16, 16));
          tile.setPosition(x * 16, y * 16);
          window->draw(tile);
        }
        else {
          continue;
        }
      }
    }
    world->player.sprite.setPosition(world->player.pos);
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
    tileart.loadFromFile("tiles.png");
  }

  Engine::~Engine() {
    delete window;
    delete world;
  }
}
