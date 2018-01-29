#include "engine.hpp"
#include "entity.hpp"
#include "fmt/format.h"
#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <stdexcept>

#include <cstddef>
#include <cstdlib>
#include <ctgmath>

namespace engine {
// Static helper method used to flip the Y axis
template<typename T>
T World::toView(const T& vector) {
  return T(vector.x, -vector.y);
}

template<typename T>
Rect<int> World::tilesFromAABB(const Rect<T>& bbox) {
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
  for (int x = 0; x < 8; x++) for (int y = 0; y < 2; y++) {
      setTile(x, y, 1);
  }
  setTile(0, 2, 1);
  for (int x = 15; x < 20; x++) {
      setTile(x, 5, 1);
  }
  setTile(5, 2, 1);
  setTile(6, 2, 1);
  setTile(6, 3, 1);
  setTile(7, 2, 1);
  setTile(7, 3, 1);
  setTile(7, 4, 1);
  for (int x = 12; x < 24; x++) for (int y = 0; y < 2; y++) {
    setTile(x, y, 1);
  }
  setTile(23, 2, 1);
  player.pos = Vector2f(32, 32);
  camera.pos = player.pos + Vector2f(0, player.height / 2);
  // End test world

  return true;
}

World::World(size_t x, size_t y) : player({7, 0}, {1, 1}, 5, 25) {
  tiles = new tileid_t[x * y]();
  size.x = x;
  size.y = y;
}

World::~World() {
  delete tiles;
}

bool Engine::Keys::State::state() const {
  return my_state;
}

byte Engine::Keys::State::delta() const {
  return my_delta;
}

void Engine::Keys::State::set(bool state) {
  my_delta = (state ? 1 : 0) - my_state;
  my_state = state ? true : false;
}

void Engine::Keys::State::press() {
  set(true);
}

void Engine::Keys::State::release() {
  set(false);
}

void Engine::Keys::State::tick() {
  my_delta = 0;
}

bool Engine::init() {
  world->init();

  textures.emplace("overworld-blocks", sf::Texture());
  textures.emplace("overworld-clouds", sf::Texture());
  textures.emplace("player.still", sf::Texture());
  textures.emplace("player.duck", sf::Texture());

  for (auto& it : textures) {
    it.second.loadFromFile(it.first + ".png");
  }

  world->player.sprite.setTexture(textures.at("player.still"));

  tick = 0;
  tickClock.restart();

  return true;
}

void Engine::resize(size_t width, size_t height) {
  window->setSize(sf::Vector2u(width, height));
}

void Engine::onKeyEvent() {
  switch (event.key.code) {
  case sf::Keyboard::Up:
    keys.up.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
    break;
  case sf::Keyboard::Left:
    keys.left.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
    break;
  case sf::Keyboard::Down:
    keys.down.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
    break;
  case sf::Keyboard::Right:
    keys.right.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
    break;
  case sf::Keyboard::X:
    keys.jump.set(sf::Keyboard::isKeyPressed(sf::Keyboard::X));
    break;
  case sf::Keyboard::Z:
    keys.run.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
  default:
    break;
  }
}

void Engine::tickKeys() {
  keys.up.tick();
  keys.left.tick();
  keys.down.tick();
  keys.right.tick();
  keys.jump.tick();
  keys.run.tick();
}

void Engine::doTick() {
  // TODO: Send these to a better place
  static const float gravity = -48 * 16;
  static const float min_yvel = -16 * 16;

  // Handle keyboard input first
  auto up = keys.up.state();
  auto left = keys.left.state();
  auto down = keys.down.state();
  auto right = keys.right.state();
  auto jump = keys.jump.state();
  auto run = keys.run.state();

  auto direction = (right ? 1 : 0) + (left ? -1 : 0);

  const float max_xvel = (run ? 10 : 6) * 16;
  const float min_xvel = -max_xvel;

  if (direction) {
    world->player.setDirection(direction);
  }

  if (not world->player.airborne) {
    if (direction and not world->player.ducking
    and world->player.vel.x > min_xvel
    and world->player.vel.x < max_xvel) {
      world->player.vel.x = std::max(min_xvel, std::min(world->player.vel.x + direction * 18 * 16 / tickRate, max_xvel));
    }
    else if (world->player.vel.x > 0) {
      world->player.vel.x = std::max(world->player.vel.x - 12 * 16 / tickRate, 0.0f);
    }
    else if (world->player.vel.x < 0) {
      world->player.vel.x = std::min(world->player.vel.x + 12 * 16 / tickRate, 0.0f);
    }

    if (down and not direction) {
      world->player.sprite.setTexture(textures.at("player.duck"), true);
      world->player.duck();
    }
    else {
      world->player.sprite.setTexture(textures.at("player.still"), true);
      world->player.stand();
    }
  }
  else {
    if (direction) {
      world->player.vel.x += direction * 6 * 16 / tickRate;
    }
  }

  if (not world->player.underwater) {
    if (keys.jump.delta() == 1) {
      if (not world->player.airborne) {
        world->player.jumptime = 0.3125;
      }
    }
    else if (keys.jump.delta() == -1) {
      world->player.jumptime = 0;
    }

    if (jump and world->player.jumptime > 0) {
      world->player.jumptime = std::max(world->player.jumptime - 1 / tickRate, 0.0f);
      world->player.jump();
    }
  }

  if (world->player.vel.x) {
    world->player.pos.x += world->player.vel.x / tickRate;
  }
  auto range = World::tilesFromAABB(world->player.getAABB());
  for (int y = range.y; y < range.y + range.h; y++)
  for (int x = range.x; x < range.x + range.w; x++) {
    auto plyrBox = world->player.getAABB();
    auto tileBox = World::tileAABB(x, y);

    if (x >= 0 and x < world->size.x
    and y >= 0 and y < world->size.y) {
      if (world->getTile(x, y) != 0
      and plyrBox.intersects(tileBox)) {
        auto collBox = plyrBox.intersection(tileBox);

        if (plyrBox.x + plyrBox.w / 2 < tileBox.x + tileBox.w / 2) {
          world->player.vel.x = 0;
          world->player.pos.x -= collBox.w;
        }
        else {
          world->player.vel.x = 0;
          world->player.pos.x += collBox.w;
        }
      }
    }
  }

  if (world->player.vel.y > min_yvel
  and world->player.jumptime == 0) {
    world->player.vel.y = std::max(world->player.vel.y + gravity / tickRate, min_yvel);
  }

  if (world->player.vel.y) {
    world->player.pos.y += world->player.vel.y / tickRate;
  }
  world->player.airborne = true;
  for (int y = range.y; y < range.y + range.h; y++)
  for (int x = range.x; x < range.x + range.w; x++) {
    auto plyrBox = world->player.getAABB();
    auto tileBox = World::tileAABB(x, y);

    if (x >= 0 and x < world->size.x
    and y >= 0 and y < world->size.y) {
      if (world->getTile(x, y) != 0
      and plyrBox.intersects(tileBox)) {
        auto collBox = plyrBox.intersection(tileBox);

        if (plyrBox.y + plyrBox.h / 2 < tileBox.y + tileBox.h / 2) {
          if (world->player.pos.x > tileBox.x + tileBox.w
          and world->getTile(x + 1, y) != 0) {
            world->setTile(x + 1, y, 0);
          }
          else {
            world->setTile(x, y, 0);
          }
          world->player.jumptime = 0;
          world->player.vel.y = 0;
          world->player.pos.y -= collBox.h;
        }
        else {
          world->player.airborne = false;
          world->player.vel.y = 0;
          world->player.pos.y += collBox.h;
        }
      }
    }
  }

  world->camera.vel = (world->player.pos + Vector2f(0, world->player.height / 2) - world->camera.pos) * tickRate / 16;

  world->camera.pos += world->camera.vel / tickRate;

  auto camLeft = window->getSize().x / 6;
  auto camRight = world->size.x * 16 - window->getSize().x / 6;
  auto camUp = world->size.y * 16 - window->getSize().y / 6;
  auto camDown = window->getSize().y / 6;

  if (world->camera.pos.x < camLeft
  and world->camera.pos.x > camRight) {
    world->camera.pos.x = (camLeft + camRight) / 2;
  }
  else {
    if (world->camera.pos.x < camLeft) {
      world->camera.pos.x = camLeft;
    }
    else if (world->camera.pos.x > camRight) {
      world->camera.pos.x = camRight;
    }
  }

  if (world->camera.pos.y > camUp
  and world->camera.pos.y < camDown) {
    world->camera.pos.y = (camUp + camDown) / 2;
  }
  else {
    if (world->camera.pos.y > camUp) {
      world->camera.vel.y = 0;
      world->camera.pos.y = camUp;
    }
    else if (world->camera.pos.y < camDown) {
      world->camera.vel.y = 0;
      world->camera.pos.y = camDown;
    }
  }

  tickKeys();
  tick++;
}

void Engine::drawBG(const char* bg, float distx, float disty) {
  const auto win_w = window->getSize().x;
  const auto win_h = window->getSize().y;

  const float distdivx = distx / (distx - 1.0f);
  const float distdivy = disty / (disty - 1.0f);
  const sf::Texture& texture = textures.at(bg);

  float left = world->camera.pos.x - win_w / 6;
  float bottom = world->camera.pos.y - win_h / 6;
  int sky_w = texture.getSize().x;
  int sky_h = texture.getSize().y;
  int min_x = left / sky_w;
  int min_y = bottom / sky_h;
  int max_x = (world->camera.pos.x + win_w * (2 * distx - 1) / 6) / sky_w + distx;
  int max_y = (world->camera.pos.y + win_h * (2 * disty - 1) / 6) / sky_h + disty;

  sf::Sprite sky(texture);
  for (int y = floor(min_y / disty); y < floor(max_y / disty) + 1; y++)
  for (int x = floor(min_x / distx); x < floor(max_x / distx) + 1; x++) {
    sky.setPosition(World::toView(Vector2f(x * sky_w + left / distdivx, y * sky_h + sky_h + bottom / distdivy)));
    window->draw(sky);
  }
}

void Engine::drawBGBottom(const char* bg, float distx, float disty) {
  const auto win_w = window->getSize().x;
  const auto win_h = window->getSize().y;

  const float distdivx = distx / (distx - 1.0f);
  const float distdivy = disty / (disty - 1.0f);
  const sf::Texture& texture = textures.at(bg);

  float left = world->camera.pos.x - win_w / 6;
  float bottom = world->camera.pos.y - win_h / 6;
  int sky_w = texture.getSize().x;
  int sky_h = texture.getSize().y;
  int min_x = left / sky_w;
  int max_x = (world->camera.pos.x + win_w * (2 * distx - 1) / 6) / sky_w + distx;

  sf::Sprite sky(texture);
  for (int x = floor(min_x / distx); x < floor(max_x / distx) + 1; x++) {
    sky.setPosition(World::toView(Vector2f(x * sky_w + left / distdivx, sky_h + bottom / distdivy)));
    window->draw(sky);
  }
}

void Engine::drawBGTop(const char* bg, float distx, float disty) {
  const auto win_w = window->getSize().x;
  const auto win_h = window->getSize().y;

  const float distdivx = distx / (distx - 1.0f);
  const float distdivy = disty / (disty - 1.0f);
  const sf::Texture& texture = textures.at(bg);

  float left = world->camera.pos.x - win_w / 6;
  float top = world->camera.pos.y + win_h / 6;
  int sky_w = texture.getSize().x;
  int min_x = left / sky_w;
  int max_x = (world->camera.pos.x + win_w * (2 * distx - 1) / 6) / sky_w + distx;

  sf::Sprite sky(texture);
  for (int x = floor(min_x / distx); x < floor(max_x / distx) + 1; x++) {
    sky.setPosition(World::toView(Vector2f(x * sky_w + left / distdivx, world->size.y * 16 / disty + top / distdivy)));
    window->draw(sky);
  }
}

void Engine::doRender() {
  const auto width = window->getSize().x;
  const auto height = window->getSize().y;

  sf::View view(Vector2f(), Vector2u(width, height) / 3);
  view.setCenter(World::toView(world->camera.pos));
  window->setView(view);

  drawBGBottom("overworld-blocks", 1.5, 1.5);
  drawBGTop("overworld-clouds", 1.625, 1.125);

  sf::Sprite brick(tileart, sf::IntRect(255, 136, 16, 16));
  for (int y = 0; y < world->size.y; y++)
  for (int x = 0; x < world->size.x; x++) {
    if (world->getTile(x, y)) {
      brick.setPosition(World::toView(Vector2f(x * 16, y * 16 + 16)));
      window->draw(brick);
    }
  }

  world->player.sprite.setPosition(world->player.toView());
  world->player.sprite.setScale(Vector2f(
    world->player.scale.x * world->player.getDirection(),
    world->player.scale.y
  ));
  window->draw(world->player.sprite);

  window->display();
}

int Engine::exec() {
  init();

  float tickDelta = 0;
  while (window->isOpen()) {
    tickDelta = tickClock.getElapsedTime().asSeconds() - tickTime;
    tickTime = tickClock.getElapsedTime().asSeconds();

    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window->close();
      }
      else if (event.type == sf::Event::Resized) {
        resize(event.size.width, event.size.height);
      }
      else if (event.type == sf::Event::KeyPressed
           or  event.type == sf::Event::KeyReleased) {
        onKeyEvent();
      }
    }

    if (int((tickTime - tickDelta) * tickRate) < int(tickTime * tickRate)) {
      doTick();
    }

    doRender();
  }

  return EXIT_SUCCESS;
}

Engine::Engine(const arglist& args) : args(args), tickRate(64) {
  window = new sf::RenderWindow(sf::VideoMode(768, 576), "Super Pixel Brawler");
  world = new World(176, 27);
  tileart.loadFromFile("tiles.png");
}

Engine::~Engine() {
  delete window;
  delete world;
}
}
