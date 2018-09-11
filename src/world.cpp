#include "world.hpp"

#include "geometry.hpp"

#include <sstream>
#include <string>

namespace ke {
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
  if (x < size.x) {
    if (y < size.y) {
      return tiles[x * size.y + y];
    }
    else {
      std::stringstream ss;
      ss << "y index out of bounds (" << y << " >= " << size.y << ")";
      throw ss.str();
    }
  }
  else {
    std::stringstream ss;
    ss << "x index out of bounds (" << x << " >= " << size.x << ")";
    throw ss.str();
  }
}

void World::setTile(int x, int y, tileid_t tileid) {
  if (x < size.x) {
    if (y < size.y) {
      getTile(x, y) = tileid;
    }
    else {
      std::stringstream ss;
      ss << "y index out of bounds (" << y << " >= " << size.y << ")";
      throw ss.str();
    }
  }
  else {
    std::stringstream ss;
    ss << "x index out of bounds (" << x << " >= " << size.x << ")";
    throw ss.str();
  }
}

World::World(int x, int y, Padding<int> padding) : padding(padding), player({7, -1}, {1, 1}, 5, 25) {
  tiles = new tileid_t[x * y]();
  size.x = x;
  size.y = y;

  // WARNING: Test world ahead
  player.pos = Vec2f(32, 16);
  camera.pos = player.pos + Vec2f(0, player.height / 2);

  setTile(0, 0, 3);
  for (int x = 1; x <= 6; x++) {
      setTile(x, 0, 4);
  }
  setTile(7, 0, 5);
  setTile(15, 4, 1);
  setTile(16, 4, 2);
  setTile(17, 4, 1);
  setTile(18, 4, 2);
  setTile(19, 4, 1);
  setTile(5, 1, 9);
  setTile(6, 1, 9);
  setTile(6, 2, 9);
  setTile(7, 1, 9);
  setTile(7, 2, 9);
  setTile(7, 3, 9);
  setTile(10, 0, 3);
  for (int x = 11; x <= size.x - 2; x++) {
    setTile(x, 0, 4);
  }
  setTile(size.x - 1, 0, 5);
  for (int x = 24; x <= 39; x++) {
    setTile(x, 1, 9);
  }
  for (int x = 28; x <= 35; x++)
  for (int y = 3; y <= 4; y++) {
    setTile(x, y, 9);
  }
  // End test world
}

World::World(int x, int y) : World(x, y, Padding<int>{}) {}

World::~World() {
  delete tiles;
}
}
