#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>

#include <cstddef>

namespace engine {
  using uint_t = unsigned int;
  using enttype_t = unsigned int;

  class BaseEntity {
  public:
    enttype_t type;
    sf::Vector2f pos;
    sf::Vector2f vel;
  };

  class RenderEntity : public BaseEntity {
  public:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f offset;
    sf::Vector2f scale;
    char facing;

    void flipX();
    void flipY();
    sf::Vector2f toView();

    RenderEntity(const sf::Vector2f&, const sf::Vector2f&);
  };

  class Entity : public RenderEntity {
  public:
    float mass;
    sf::Vector2f netForce;

    void applyForce(const float, const float);
    void applyForce(const sf::Vector2f&);

    Entity(const float, const sf::Vector2f&, const sf::Vector2f&);
  };

  class PlayerEntity : public Entity {
  public:
    struct State {
      bool jumping, crouching, underwater;
    } state;

    PlayerEntity(const float, const sf::Vector2f&, const sf::Vector2f&);
  };

  using tileid_t = unsigned int;

  class World {
  public:
    tileid_t* tiles;
    sf::Vector2<size_t> size;
    PlayerEntity player;
    BaseEntity camera;

    template<typename T>
    static T toView(const T&);
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
    uint_t tick;
    sf::Clock tickClock;
    float tickTime, tickRate;
    sf::Event event;
    sf::RenderWindow* window;
    sf::Texture background;
    sf::Texture tileart;
    World* world;
    struct Keys {
      bool up, left, down, right;
      bool jump, run;
    } keys;

    bool init();
    void updateKeys();
    void resize(const size_t, const size_t);
    void onTick();
    void render();

  public:
    int exec();
    Engine(const arglist&);
    ~Engine();
  };
}

#endif
