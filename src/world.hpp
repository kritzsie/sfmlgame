#ifndef WORLD_HPP
#define WORLD_HPP

#include "entity.hpp"
#include "geometry.hpp"

namespace world {
using namespace entity;
using namespace types;

using tileid_t = uint;

using geometry::vec2::Vec2;
using Vec2f = Vec2<float>;

using geometry::rect::Rect;

class World {
protected:
  tileid_t* tiles;

public:
  Vec2<int> size;

  BaseEntity camera;
  PlayerEntity player;

  static Vec2f toView(const Vec2f&);
  static Rect<int> tilesFromAABB(const Rect<float>&);
  static Rect<float> tileAABB(int, int);
  tileid_t& getTile(int, int);
  void setTile(int, int, tileid_t);
  bool init();

  World(int, int);
  ~World();
};
}

namespace keng {
  using namespace world;
}

#endif // WORLD_HPP
