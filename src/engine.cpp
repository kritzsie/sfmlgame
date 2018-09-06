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
TimeInfo::TimeInfo(float rate) : rate(rate), delta(1.0f / rate) {}

TimeInfo::TimeInfo() : TimeInfo(60.0f) {}

/*
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
*/

void Engine::onResize(Vec2<uint> size) {
  fbscale = std::max(1u, std::min(size.x / fbsize.x, size.y / fbsize.y));
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
    inputs[Actions::up] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    break;
  case sf::Keyboard::Left:
    inputs[Actions::left] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    break;
  case sf::Keyboard::Down:
    inputs[Actions::down] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    break;
  case sf::Keyboard::Right:
    inputs[Actions::right] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    break;
  case sf::Keyboard::X:
    inputs[Actions::jump] = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    break;
  case sf::Keyboard::Z:
    inputs[Actions::run] = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
  default:
    break;
  }
}

void Engine::updateInputs() {
  inputs[Actions::up].update();
  inputs[Actions::left].update();
  inputs[Actions::down].update();
  inputs[Actions::right].update();
  inputs[Actions::jump].update();
  inputs[Actions::run].update();
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

void Engine::pushState(GameState::Factory state_factory) {
  events.push_back(Event(EventType::push, state_factory));
}

GameState* Engine::popState() {
  events.push_back(Event(EventType::pop, nullptr));
  return states.back();
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
      case EventType::push: {
        GameState* state = event.second(*this);
        states.push_back(state);
        state->enter();
        break;
      }
      case EventType::pop: {
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

    handleEvents();

    if (nexttick < curtime) {
      update();
      nexttick += 1.0s / ticktime.rate;
      updateInputs();
    }

    if (nextrender < curtime) {
      draw();
      nextrender += 1.0s / rendertime.rate;
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
  setupPhysFS("Kha0z", "smb3", "basesmb3");

  auto videomode = sf::VideoMode::getDesktopMode();
  uint scale = std::max(1, std::min<int>(videomode.width / fbsize.x, videomode.height / fbsize.y) - 1);
  uint width = fbsize.x * scale;
  uint height = fbsize.y * scale;

  window->create(sf::VideoMode(width, height), "Super Mario Bros. 3");
  window->setPosition(sf::Vector2i((videomode.width - width) / 2, (videomode.height - height) / 2));

  pushState(Intro::makeState());

  return true;
}

Engine::Engine(const StringList& args) : args(args), ticktime(64.0f), fbsize(480, 270) {
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
