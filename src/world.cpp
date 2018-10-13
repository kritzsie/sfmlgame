#include "world.hpp"

#include "engine.hpp"
#include "geometry.hpp"
#include "states/basegame.hpp"

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

Vec2i World::getSize() const {
  return tiles.size;
}

tileid_t& World::getTile(int x, int y) {
  return tiles[x][y];
}

void World::setTile(int x, int y, tileid_t tileid) {
  tiles[x][y] = tileid;
}

BaseEntity* World::spawnEntity(BaseEntity::Factory factory) {
  BaseEntity* entity = factory(engine, this);
  entities.push_back(entity);
  return entity;
}

void World::triggerCoin(int x, int y) {
  setTile(x, y, 0);
  if (basegame->getCoins() >= 99) {
    basegame->addLives(1);
    engine->sound->play("1up");
  }
  basegame->addCoins(1);
  basegame->addScore(50);
  engine->sound->play("coin");
}

void World::update() {
  for (BaseEntity* entity : entities) {
    entity->update();
  }

  timer = std::max(0.f, timer - 1.f / engine->ticktime.rate);
}

World::World(Engine* engine, BaseGame* basegame, int x, int y, Padding<int> padding)
: tiles(x, y), engine(engine), basegame(basegame), padding(padding) {
  // WARNING: Test world ahead
  player = dynamic_cast<Player*>(spawnEntity(Player::create()));
  camera = dynamic_cast<Camera*>(spawnEntity(Camera::create()));

  player->pos = Vec2(32.f, 16.f);
  camera->setTarget(player);

  setTile(0, 0, 3);
  for (int x = 1; x <= 6; x++) {
    setTile(x, 0, 4);
  }

  setTile(7, 0, 5);

  // brick & item block platform
  setTile(15, 4, 1);
  setTile(16, 4, 2);
  setTile(17, 4, 1);
  setTile(18, 4, 2);
  setTile(19, 4, 1);

  // row of coins
  setTile(29, 8, 10);
  setTile(30, 8, 10);
  setTile(31, 8, 10);
  setTile(32, 8, 10);
  setTile(33, 8, 10);
  setTile(34, 8, 10);

  setTile(5, 1, 9);
  setTile(6, 1, 9);
  setTile(6, 2, 9);
  setTile(7, 1, 9);
  setTile(7, 2, 9);
  setTile(7, 3, 9);
  setTile(10, 0, 3);
  for (int x = 11; x <= tiles.size.x - 2; x++) {
    setTile(x, 0, 4);
  }
  setTile(tiles.size.x - 1, 0, 5);
  for (int x = 24; x <= 39; x++) {
    setTile(x, 1, 9);
  }
  for (int x = 28; x <= 35; x++)
  for (int y = 3; y <= 4; y++) {
    setTile(x, y, 9);
  }
  // End test world
}

World::World(Engine* engine, BaseGame* basegame, int x, int y)
: World(engine, basegame, x, y, Padding<int>{}) {}
}
