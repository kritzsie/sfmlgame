#include "engine.hpp"

#include "assetmanager.hpp"
#include "geometry.hpp"
#include "renderer.hpp"
#include "tiletypes.hpp"
#include "types.hpp"
#include "world.hpp"

#include <physfs.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <thread>

namespace ke {
TimeInfo::TimeInfo(float rate) : rate(rate), delta(1.f / rate) {}

TimeInfo::TimeInfo() : TimeInfo(60.f) {}

void Engine::onResize(Vec2<uint> size) {
  viewscale = std::max(1u, std::min(size.x / viewsize.x, size.y / viewsize.y));
  uint width = viewsize.x * viewscale;
  uint height = viewsize.y * viewscale;

  window->setView(sf::View(
    sf::Vector2f(width / 2.f, height / 2.f),
    sf::Vector2f(size.x, size.y)
  ));

  viewport->create(width, height);
}

void Engine::onKeyEvent(sf::Event& event) {
  switch (event.key.code) {
  case sf::Keyboard::Up:
    inputs[Action::up] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    break;
  case sf::Keyboard::Left:
    inputs[Action::left] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    break;
  case sf::Keyboard::Down:
    inputs[Action::down] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    break;
  case sf::Keyboard::Right:
    inputs[Action::right] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    break;
  case sf::Keyboard::X:
    inputs[Action::jump] = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    break;
  case sf::Keyboard::Z:
    inputs[Action::run] = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    break;
  case sf::Keyboard::Escape:
    inputs[Action::pause] = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
  default:
    break;
  }
}

bool Engine::setupPhysFS(std::string org, std::string appname, std::string basegame) {
  PHYSFS_permitSymbolicLinks(true);

  std::string prefdir = PHYSFS_getPrefDir(org.c_str(), appname.c_str());

  if (PHYSFS_setSaneConfig(org.c_str(), appname.c_str(), nullptr, 0, 0) == 0) {
    return false;
  }

  if (PHYSFS_mount((prefdir + basegame).c_str(), ("/games/" + basegame).c_str(), 1)) {
    PHYSFS_mount((prefdir + basegame + "/maps").c_str(), "/maps", 1);
    PHYSFS_mount((prefdir + basegame + "/music").c_str(), "/music", 1);
    PHYSFS_mount((prefdir + basegame + "/sounds").c_str(), "/sounds", 1);
    PHYSFS_mount((prefdir + basegame + "/sprites").c_str(), "/sprites", 1);
    PHYSFS_mount((prefdir + basegame + "/textures").c_str(), "/textures", 1);
    PHYSFS_mount((prefdir + basegame + "/tiles").c_str(), "/tiles", 1);
  }
  else {
    return false;
  }

  return true;
}

void Engine::pushState(GameState::Factory factory) {
  events.push_back(Event(EventType::pushState, factory));
}

GameState* Engine::popState() {
  events.push_back(Event(EventType::popState, nullptr));
  return states.back();
}

TileRedefinitionError::TileRedefinitionError(const std::string& what)
: std::logic_error(what) {}

TileRedefinitionError::TileRedefinitionError(const char* what)
: TileRedefinitionError(std::string(what)) {}

void Engine::registerTileDef(tileid_t tileid, TileDef tiledef) {
  if (tiles.find(tileid) != tiles.end()) {
    std::stringstream ss;
    ss << "attempted to redefine tile with id " << tileid;
    throw TileRedefinitionError(ss.str());
  }

  tiles.emplace(tileid, tiledef);
}

const TileDef& Engine::getTileDef(tileid_t tileid) const {
  return tiles.at(tileid);
}

void Engine::handleEvents() {
  sf::Event event;
  while (window->pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      window->close();
      break;
    case sf::Event::Resized:
      onResize(Vec2(event.size.width, event.size.height));
      break;
    case sf::Event::KeyPressed:
    case sf::Event::KeyReleased:
      onKeyEvent(event);
    default:
      break;
    }
  }
}

void Engine::update() {
  for (Event& event : events) {
    switch (event.first) {
      case EventType::pushState: {
        GameState* state = event.second(this);
        states.push_back(state);
        state->enter();
        break;
      }
      case EventType::popState: {
        GameState* state = states.back();
        state->exit();
        delete state;
        states.pop_back();
        break;
      }
    }
  }

  if (events.size() > 0) {
    events.clear();
  }

  for (GameState* state : states) {
    state->update();
  }

  for (auto& it : inputs) {
    it.second.update();
  }
}

void Engine::draw() {
  window->clear();

  for (GameState* state : states) {
    state->draw();
  }

  window->draw(sf::Sprite(viewport->getTexture()));
  window->display();
}

int Engine::main() {
  if (init() == false) {
    return EXIT_FAILURE;
  }

  using namespace std::literals::chrono_literals;

  auto starttime = Clock::now();
  auto prevtime = starttime;
  auto nexttick = prevtime + 1.s / ticktime.rate;
  auto nextrender = prevtime + 1.s / rendertime.rate;

  while (window->isOpen()) {
    Clock::time_point curtime = Clock::now();

    handleEvents();

    if (nexttick < curtime) {
      update();
      nexttick += 1.s / ticktime.rate;
    }

    if (nextrender < curtime) {
      draw();
      nextrender += 1.s / rendertime.rate;
    }

    if (events.size() > 0
    or  states.size() > 0) {
      std::this_thread::sleep_until(std::min(nexttick, nextrender));
      prevtime = curtime;
    }
    else {
      window->close();
    }
  }

  return EXIT_SUCCESS;
}

bool Engine::init() {
  setupPhysFS("Klay", "smb3", "basesmb3");

  sf::VideoMode videomode = sf::VideoMode::getDesktopMode();
  uint scale = std::max(1, std::min<int>(videomode.width / viewsize.x,
                                         videomode.height / viewsize.y) - 1);
  uint width = viewsize.x * scale;
  uint height = viewsize.y * scale;

  window->create(sf::VideoMode(width, height), "Super Mario Bros. 3");
  window->setPosition(sf::Vector2i((videomode.width - width) / 2,
                                   (videomode.height - height) / 2));

  TileDef brickblock;
  brickblock.pushFrame("smb3_tile_atlas", Vec2(0, 0), 8.f / 60.f);
  brickblock.pushFrame("smb3_tile_atlas", Vec2(16, 0), 8.f / 60.f);
  brickblock.pushFrame("smb3_tile_atlas", Vec2(32, 0), 8.f / 60.f);
  brickblock.pushFrame("smb3_tile_atlas", Vec2(48, 0), 8.f / 60.f);
  registerTileDef(1, brickblock);

  TileDef itemblock;
  itemblock.pushFrame("smb3_tile_atlas", Vec2(0, 48), 8.f / 60.f);
  itemblock.pushFrame("smb3_tile_atlas", Vec2(16, 48), 8.f / 60.f);
  itemblock.pushFrame("smb3_tile_atlas", Vec2(32, 48), 8.f / 60.f);
  itemblock.pushFrame("smb3_tile_atlas", Vec2(48, 48), 8.f / 60.f);
  registerTileDef(2, itemblock);

  TileDef woodfloor0, woodfloor1, woodfloor2;
  TileDef woodfloor3, woodfloor4, woodfloor5;
  woodfloor0.pushFrame("smb3_tile_atlas", Vec2(80, 48), 0.f);
  woodfloor1.pushFrame("smb3_tile_atlas", Vec2(96, 48), 0.f);
  woodfloor2.pushFrame("smb3_tile_atlas", Vec2(112, 48), 0.f);
  woodfloor3.pushFrame("smb3_tile_atlas", Vec2(80, 48), 0.f);
  woodfloor4.pushFrame("smb3_tile_atlas", Vec2(96, 48), 0.f);
  woodfloor5.pushFrame("smb3_tile_atlas", Vec2(112, 48), 0.f);
  registerTileDef(3, woodfloor0);
  registerTileDef(4, woodfloor1);
  registerTileDef(5, woodfloor2);
  registerTileDef(6, woodfloor3);
  registerTileDef(7, woodfloor4);
  registerTileDef(8, woodfloor5);

  TileDef woodblock;
  woodblock.pushFrame("smb3_tile_atlas", Vec2(208, 96), 0.f);
  registerTileDef(9, woodblock);

  pushState(Intro::create());

  return true;
}

Engine::Engine(const StringList& args)
: args(args), ticktime(64.f), rendertime(60.f), viewsize(480, 270) {
  if (PHYSFS_isInit() == 0) {
    PHYSFS_init(args.at(0).c_str());
    deinitPhysFS = true;
  }

  viewport = new sf::RenderTexture;
  window = new sf::RenderWindow;

  music = new Music;
  sound = new Sound;

  TileDef notile(TileType::NONE);
  notile.pushFrame("", Vec2i(), 0.f);
  registerTileDef(0, notile);
}

Engine::~Engine() {
  if (window   != nullptr) delete window;
  if (viewport != nullptr) delete viewport;
  if (music    != nullptr) delete music;
  if (sound    != nullptr) delete sound;

  if (deinitPhysFS) {
    PHYSFS_deinit();
  }
}
}
