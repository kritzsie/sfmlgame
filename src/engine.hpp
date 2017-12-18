#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <cstddef>

#include <vector>

namespace engine {
  class BaseEntity {
  public:
    sf::Vector2f pos;
    sf::Vector2f vel;

    BaseEntity();
  };

  class Entity : public BaseEntity {
  public:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f netForce;
    float mass;

    void applyForce(const float, const float);
    void applyForce(const sf::Vector2f&);

    Entity(const float);
  };

  using tileid_t = unsigned int;

  class World {
  public:
    tileid_t* tiles;
    sf::Vector2<size_t> size;
    Entity player;
    BaseEntity camera;

    bool init();
    tileid_t& getTile(const int, const int);
    void setTile(const int, const int, const tileid_t);

    World(const size_t, const size_t);
    ~World();
  };

  using arglist = std::vector<const char*>;

  class Engine {
  private:
    const arglist& args;
    sf::Clock tickClock;
    float tickTime, tickRate;
    sf::Event event;
    sf::RenderWindow* window;
    sf::Texture tileart;
    World* world;

    bool init();
    void resize(const size_t width, const size_t height);
    void tick();
    void render();

  public:
    int run();
    Engine(const arglist&);
    ~Engine();
  };
}

#endif
