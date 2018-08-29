#include "engine.hpp"

#include "assetmanager.hpp"
#include "geometry.hpp"
#include "types.hpp"
#include "world.hpp"

#include <physfs.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <thread>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctgmath>

namespace ke {
TimeInfo::TimeInfo(float rate) : rate(rate), delta(1 / rate) {}

TimeInfo::TimeInfo() : TimeInfo(60) {}

bool Keys::State::getState() const {
  return state;
}

byte Keys::State::getDelta() const {
  return delta;
}

void Keys::State::setState(bool pressed) {
  delta = (pressed ? 1 : 0) - state;
  state = pressed ? true : false;
}

void Keys::State::press() {
  setState(true);
}

void Keys::State::release() {
  setState(false);
}

void Keys::State::resetDelta() {
  delta = 0;
}

void Engine::onResize(Vec2<uint> size) {
  fbscale = std::max<uint>(1, std::min(size.x / fbsize.x, size.y / fbsize.y));

  uint width = fbsize.x * fbscale;
  uint height = fbsize.y * fbscale;

  window->setView(sf::View(
    sf::Vector2f(width / 2.0f, height / 2.0f),
    sf::Vector2f(size.x, size.y)
  ));

  viewport->create(width, height);
}

void Engine::onKeyEvent(sf::Event& event) {
  switch (event.key.code) {
  case sf::Keyboard::Up:
    keys.up.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
    break;
  case sf::Keyboard::Left:
    keys.left.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
    break;
  case sf::Keyboard::Down:
    keys.down.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
    break;
  case sf::Keyboard::Right:
    keys.right.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
    break;
  case sf::Keyboard::X:
    keys.jump.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::X));
    break;
  case sf::Keyboard::Z:
    keys.run.setState(sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
  default:
    break;
  }
}

void Engine::tickKeys() {
  keys.up.resetDelta();
  keys.left.resetDelta();
  keys.down.resetDelta();
  keys.right.resetDelta();
  keys.jump.resetDelta();
  keys.run.resetDelta();
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

void Engine::handleEvents() {
  sf::Event event;
  while (window->pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      window->close();
      break;
    case sf::Event::Resized:
      onResize(Vec2<uint>(event.size.width, event.size.height));
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
  for (auto state : states) {
    state->update();
  }
}

void Engine::draw() {
  window->clear();

  for (auto state : states) {
    state->draw();
  }

  window->draw(sf::Sprite(viewport->getTexture()));
  window->display();
}

bool Engine::init() {
  setupPhysFS("Kha0z", "smb3", "basesmb3");

  auto videomode = sf::VideoMode::getDesktopMode();
  uint scale = std::max<uint>(1, std::min(videomode.width / fbsize.x, videomode.height / fbsize.y) - 1);
  uint width = fbsize.x * scale;
  uint height = fbsize.y * scale;

  window->create(sf::VideoMode(width, height), "Super Mario Bros. 3");
  window->setPosition(sf::Vector2i((videomode.width - width) / 2, (videomode.height - height) / 2));

  music->change("overworld");
  music->play();

  states.push_back(new Intro(this));

  return true;
}

int Engine::main() {
  if (init() == false) {
    return EXIT_FAILURE;
  }

  using namespace std::literals::chrono_literals;

  auto starttime = Clock::now();
  auto prevtime = starttime;
  auto nexttick = prevtime + 1.0s / ticktime.rate;
  auto nextrender = prevtime + 1.0s / rendertime.rate;

  while (window->isOpen()) {
    Clock::time_point curtime = Clock::now();
    //Clock::duration deltatime = curtime - prevtime;

    handleEvents();

    if (nexttick < curtime) {
      update();
      nexttick += 1.0s / ticktime.rate;
    }

    if (nextrender < curtime) {
      draw();
      nextrender += 1.0s / rendertime.rate;
    }

    std::this_thread::sleep_until(std::min(nexttick, nextrender));

    prevtime = curtime;
  }

  return EXIT_SUCCESS;
}

Engine::Engine(const StringList& args) : args(args), ticktime(64), fbsize(480, 270) {
  if (PHYSFS_isInit() == 0) {
    PHYSFS_init(args.at(0).c_str());
    deinitPhysFS = true;
  }

  viewport = new sf::RenderTexture();
  window   = new sf::RenderWindow();
  music    = new Music();
  sound    = new Sound();
}

Engine::~Engine() {
  if (deinitPhysFS) PHYSFS_deinit();

  if (window != nullptr) delete window;
  if (music  != nullptr) delete music;
  if (sound  != nullptr) delete sound;
}
}
