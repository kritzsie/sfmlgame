#pragma once

#include "entities.hpp"
#include "geometry.hpp"

#include <cstdint>
#include <list>

namespace ke {
class Engine;

class BaseGame;

using tileid_t = uint32_t;

template<typename T>
struct Padding {
  T left, right, bottom, top;
};

class World {
public:
  class State {
  public:
    std::list<BaseEntity*> entities;

    float gravity = -768.f;
    float timer = 300.f;
  };

  class States {
  public:
    State cur, next;

    void update();
  };

protected:
  Engine* const engine = nullptr;
  BaseGame* const basegame = nullptr;

private:
  Matrix<tileid_t> tiles;
  States states;

public:
  Padding<int> padding;

  Camera* camera = nullptr;
  Player* player = nullptr;

  static Vec2f toView(Vec2f);
  static Vec2f toView(RenderEntity*);

  static Rect<int> tilesInBBox(const Rect<float>&);
  static Rect<float> tileBBox(int, int);

  Vec2i getSize() const;

  tileid_t& getTile(int, int);
  void setTile(int, int, tileid_t);

  const std::list<BaseEntity*>& getEntities() const;
  BaseEntity* spawnEntity(BaseEntity::Factory);

  float getGravity() const;
  void setGravity(float);

  float getTimer() const;

  void triggerCoin(int, int);

  void update();

  World(Engine*, BaseGame*, int, int);
  World(Engine*, BaseGame*, int, int, Padding<int>);
};
}
