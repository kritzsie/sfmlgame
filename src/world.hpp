#ifndef WORLD_HPP
#define WORLD_HPP

#include "entity.hpp"
#include "geometry.hpp"

namespace world {
using namespace entity;
using namespace types;

using tileid_t = uint;

using geometry::vector2::Vector2;
using Vector2f = Vector2<float>;
using Vector2u = Vector2<uint>;

using geometry::rect::Rect;

class World {
protected:
  tileid_t* tiles;

public:
  Vector2<size_t> size;

  BaseEntity camera;
  PlayerEntity player;

  static Vector2f toView(const Vector2f&);
  static Rect<int> tilesFromAABB(const Rect<float>&);
  static Rect<float> tileAABB(int, int);
  tileid_t& getTile(int, int);
  void setTile(int, int, tileid_t);
  bool init();

  World(size_t, size_t);
  ~World();
};
}

namespace keng {
  using namespace world;
}

#endif // WORLD_HPP
