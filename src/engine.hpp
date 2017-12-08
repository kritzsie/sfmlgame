#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <chrono>
#include <thread>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace engine {
  using arglist = std::vector<const char*>;

  class Entity {
  private:
    sf::Vector2f netForce;

  public:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;
    sf::Vector2f vel;
    float mass;
    bool flying;

    void addForce(float, float);
    void addForce(sf::Vector2f);
  };

  using tileid_t = unsigned int;

  class World {
  public:
    tileid_t *tiles;
    sf::Vector2<size_t> size;

    bool init();
    tileid_t& getTile(int, int);
    void setTile(int, int, const tileid_t);

    tileid_t& operator()(int, int);

    World(size_t, size_t);
    ~World();
  };

  class Engine {
  private:
    const arglist& args;
    sf::Clock timer;
    sf::Event event;
    sf::RenderWindow* window;
    sf::Texture tileart;
    Entity player;
    World* world;

    bool init();
    void resize(int width, int height);
    void tick();
    void render();

  public:
    int run();
    Engine(const arglist&);
    ~Engine();
  };
}

#endif
