#include "geometry.hpp"
#include "world.hpp"

namespace world {
Vector2f World::toView(const Vector2f& vector) {
  return Vector2f(vector.x, -vector.y);
}

Rect<int> World::tilesFromAABB(const Rect<float>& bbox) {
  return Rect<int>(
    floor(bbox.x / 16) - 1, floor(bbox.y / 16) - 1,
    ceil(bbox.w / 16) + 2, ceil(bbox.h / 16) + 2
  );
}

Rect<float> World::tileAABB(int x, int y) {
  return Rect<float>(x * 16, y * 16, 16, 16);
}

// TODO: Inherit from dedicated 2D vector class
tileid_t& World::getTile(int x, int y) {
  if (x < size.x)
    if (y < size.y)
      return tiles[x * size.y + y];
    else throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
  else throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

void World::setTile(int x, int y, tileid_t tileid) {
  if (x < size.x)
    if (y < size.y)
      getTile(x, y) = tileid;
    else throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
  else throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

bool World::init() {
  // WARNING: Test world ahead
  for (int x = 0; x < 8; x++) for (int y = 0; y < 1; y++) {
      setTile(x, y, 1);
  }
  setTile(0, 1, 1);
  for (int x = 15; x < 20; x++) {
      setTile(x, 4, 1);
  }
  setTile(5, 1, 1);
  setTile(6, 1, 1);
  setTile(6, 2, 1);
  setTile(7, 1, 1);
  setTile(7, 2, 1);
  setTile(7, 3, 1);
  for (int x = 12; x < size.x; x++) for (int y = 0; y < 1; y++) {
    setTile(x, y, 1);
  }
  setTile(23, 1, 1);
  player.pos = Vector2f(32, 16);
  camera.pos = player.pos + Vector2f(0, player.height / 2);
  // End test world

  return true;
}

World::World(int x, int y) : player({7, 0}, {1, 1}, 5, 25) {
  tiles = new tileid_t[x * y]();
  size.x = x;
  size.y = y;
}

World::~World() {
  delete tiles;
}
}
