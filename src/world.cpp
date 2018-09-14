#include "world.hpp"

#include "geometry.hpp"

#include <cmath>
#include <sstream>
#include <string>

namespace ke {
Vec2f World::toView(Vec2f vector) {
  return Vec2(vector.x, -vector.y);
}

Vec2f World::toView(RenderEntity* entity) {
  const RenderFrame& frame = entity->getFrame();

  return toView(Vec2(
    entity->pos.x - (frame.offset.x * entity->scale.x) * entity->getDirection(),
    entity->pos.y + (frame.offset.y * entity->scale.y) + frame.cliprect.h * entity->scale.y
  ));
}

Rect<int> World::tilesInBBox(const Rect<float>& bbox) {
  return Rect<int>(
    std::floor(bbox.x / 16 - 1), std::floor(bbox.y / 16 - 1),
    std::ceil(bbox.w / 16 + 1), std::ceil(bbox.h / 16 + 1)
  );
}

Rect<float> World::tileBBox(int x, int y) {
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

BaseEntity* World::spawnEntity(BaseEntity::Factory factory) {
  BaseEntity* entity = factory(engine, this);
  entities.push_back(entity);
  return entity;
}

World::World(Engine* engine, int x, int y, Padding<int> padding)
: engine(engine), size(x, y), padding(padding) {
  tiles = new tileid_t[x * y]();

  // WARNING: Test world ahead
  player = dynamic_cast<Player*>(spawnEntity(Player::create()));
  camera = dynamic_cast<Camera*>(spawnEntity(Camera::create()));

  player->pos = Vec2(32.f, 16.f);
  camera->pos = player->pos;
  camera->setTarget(player);

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

World::World(Engine* engine, int x, int y)
: World(engine, x, y, Padding<int>{}) {}

World::~World() {
  delete tiles;
}
}
