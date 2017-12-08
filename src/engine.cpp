//#include <chrono>
#include <iostream>
//#include <sstream>
#include <stdexcept>
//#include <string>
#include <sstream>
//#include <thread>

//#include <cstdlib>
//#include <ctgmath>

//#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/RenderWindow.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>

#include "engine.hpp"

namespace engine {
  bool World::init() {
    #if true
    for (int x = 0; x < 4; x++) {
      for (int y = 6; y < 8; y++) {
        setTile(x, y, 1);
      }
    }
    setTile(0, 2, 1);
    setTile(size.x - 1, size.y - 1, 1);
    #endif
    return true;
  }

  tileid_t& World::getTile(int x, int y) {
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

  void World::setTile(int x, int y, tileid_t tileid) {
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

  tileid_t& World::operator()(int x, int y) {
    return getTile(x, y);
  }

  World::World(size_t x, size_t y) {
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
    timer.restart();
    player.texture.loadFromFile("player.png");
    player.sprite.setTexture(player.texture);
    return true;
  }

  void Engine::resize(int width, int height) {}

  void Engine::tick() {
    while (window->pollEvent(event)) {
      if      (event.type == sf::Event::Closed)  window->close();
      else if (event.type == sf::Event::Resized) resize(event.size.width, event.size.height);
    }
  }

  void Engine::render() {
    auto width  = window->getSize().x;
    auto height = window->getSize().y;

    sf::Sprite tile(tileart);
    for (int x = 0; x < world->size.x; x++) {
      for (int y = 0; y < world->size.y; y++) {
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
    player.sprite.setPosition(player.pos);
    window->draw(player.sprite);
    window->display();
  }

  int Engine::run() {
    init();

    float time = timer.getElapsedTime().asSeconds();
    while (window->isOpen()) {
      auto delta = timer.getElapsedTime().asSeconds() - time;
      time = timer.getElapsedTime().asSeconds();
      tick();
      render();
    }

    return EXIT_SUCCESS;
  }

  Engine::Engine(const arglist& args) : args(args) {
    window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Super Pixel Brawler");
    world = new World(64, 32);
    tileart.loadFromFile("tiles.png");
  }

  Engine::~Engine() {
    delete window;
    delete world;
  }
}
