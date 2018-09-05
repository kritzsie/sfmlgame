#pragma once

#include "entity.hpp"
#include "geometry.hpp"

namespace ke {
using tileid_t = uint;

template<typename T>
struct Padding {
  T left, right, bottom, top;
};

class World {
protected:
  tileid_t* tiles;

public:
  Vec2<int> size;
  Padding<int> padding;

  BaseEntity camera;
  PlayerEntity player;

  static Vec2f toView(const Vec2f&);
  static Rect<int> tilesFromAABB(const Rect<float>&);
  static Rect<float> tileAABB(int, int);

  void enter();
  void exit();

  tileid_t& getTile(int, int);
  void setTile(int, int, tileid_t);

  World(int, int);
  World(int, int, Padding<int>);
  ~World();
};
}
