#include "geometry.hpp"
#include "world.hpp"

#include <fmt/format.h>

namespace world {
Vec2f World::toView(const Vec2f& vector) {
  return Vec2f(vector.x, -vector.y);
}

Rect<int> World::tilesFromAABB(const Rect<float>& bbox) {
  return Rect<int>(
    floor(bbox.x / 16 - 1), floor(bbox.y / 16 - 1),
    ceil(bbox.w / 16 + 1), ceil(bbox.h / 16 + 1)
  );
}

Rect<float> World::tileAABB(int x, int y) {
  return Rect<float>(x * 16, y * 16, 16, 16);
}

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
  setTile(0, 0, 5);
  for (int x = 1; x <= 6; x++) {
      setTile(x, 0, 6);
  }
  setTile(7, 0, 7);
  setTile(15, 4, 30);
  setTile(16, 4, 2);
  setTile(17, 4, 30);
  setTile(18, 4, 2);
  setTile(19, 4, 30);
  setTile(5, 1, 37);
  setTile(6, 1, 37);
  setTile(6, 2, 37);
  setTile(7, 1, 37);
  setTile(7, 2, 37);
  setTile(7, 3, 37);
  for (int x = 12; x < size.x; x++) for (int y = 0; y < 1; y++) {
    setTile(x, y, 1);
  }
  for (int x = 24; x < 40; x++) {
    setTile(x, 1, 1);
  }
  for (int x = 28; x < 36; x++) {
    setTile(x, 3, 1);
    setTile(x, 4, 1);
  }
  setTile(23, 1, 1);
  player.pos = Vec2f(32, 16);
  camera.pos = player.pos + Vec2f(0, player.height / 2);
  // End test world

  return true;
}

World::World(int x, int y) : player({7, -1}, {1, 1}, 5, 25) {
  tiles = new tileid_t[x * y]();
  size.x = x;
  size.y = y;
}

World::~World() {
  delete tiles;
}
}
