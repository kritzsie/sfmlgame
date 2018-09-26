#pragma once

#include "entities.hpp"
#include "geometry.hpp"

#include <cstdint>
#include <list>

namespace ke {
class Engine;

using tileid_t = uint32_t;

template<typename T>
struct Padding {
  T left, right, bottom, top;
};

class World {
private:
  tileid_t* tiles = nullptr;

protected:
  Engine* const engine = nullptr;

public:
  const Vec2<int> size;
  Padding<int> padding;

  float gravity = -768.f;
  float timer = 300.f;

  std::list<BaseEntity*> entities;
  Camera* camera = nullptr;
  Player* player = nullptr;

  static Vec2f toView(Vec2f);
  static Vec2f toView(RenderEntity*);

  static Rect<int> tilesInBBox(const Rect<float>&);
  static Rect<float> tileBBox(int, int);

  tileid_t& getTile(int, int);
  void setTile(int, int, tileid_t);

  BaseEntity* spawnEntity(BaseEntity::Factory);

  void update();

  World(Engine*, int, int);
  World(Engine*, int, int, Padding<int>);
  ~World();
};
}
