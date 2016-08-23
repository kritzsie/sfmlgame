#include <chrono>
#include <sstream>
#include <string>
#include <thread>

#include <cstdlib>

//#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
//#include <SFML/Graphics/Text.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "engine.hpp"

namespace engine {
  bool Engine::init() {
    window->setVisible(true);
  }

  void Engine::tick() {
    auto event = sf::Event();

    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) window->close();
    }
  }

  void Engine::render() {
    window->clear(sf::Color::Black);

    window->display();
  }

  int Engine::exec() {
    init();

    while (window->isOpen()) {
      tick();
      render();
    }

    return EXIT_SUCCESS;
  }

  Engine::Engine(const arglist& args) : args(args) {
    window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "RPG Tile Engine", sf::Style::Fullscreen);
    window->setVisible(false);
  }

  Engine::~Engine() {
    delete window;
  }
}
