#pragma once

#include "assetmanager.hpp"
#include "geometry.hpp"
#include "input.hpp"
#include "music.hpp"
#include "sound.hpp"
#include "states.hpp"
#include "types.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <chrono>
#include <map>
#include <string>
#include <vector>

#include <cstddef>
#include <cstdint>

namespace ke {
using Clock = std::chrono::steady_clock;

class TimeInfo {
public:
  float rate;
  float delta;

  TimeInfo();
  TimeInfo(float);
};

class Engine {
public:
  enum class EventType {
    push, pop
  };

  using Event = std::pair<EventType, GameState::Factory>;

private:
  bool deinitPhysFS = false;

  sf::RenderWindow* window = nullptr;

  std::vector<Event> events;
  std::vector<GameState*> states;

public:
  StringList args;

  InputMap inputs;

  TimeInfo ticktime;
  TimeInfo rendertime;

  Music* music = nullptr;
  Sound* sound = nullptr;

  sf::RenderTexture* viewport = nullptr;
  Vec2<uint> viewsize;
  uint viewscale;

protected:
  virtual void onKeyEvent(sf::Event&);
  virtual void onResize(Vec2<uint>);

public:
  bool setupPhysFS(std::string, std::string, std::string);

  void pushState(GameState::Factory);
  GameState* popState();

  void handleEvents();
  void update();
  void draw();

  bool init();
  int main();

  Engine(const StringList&);
  ~Engine();
};
}
