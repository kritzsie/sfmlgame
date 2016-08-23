#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <thread>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace engine {
  typedef std::vector<const char*> arglist;

  class Engine {
  private:
    const arglist& args;
    sf::RenderWindow* window = nullptr;

    bool init();
    void tick();
    void render();

  public:
    int exec();
    Engine(const arglist&);
    ~Engine();
  };
}

#endif
