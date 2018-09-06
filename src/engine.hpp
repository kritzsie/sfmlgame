#pragma once

#include "assetmanager.hpp"
#include "geometry.hpp"
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

class GameState;

struct Keys {
  class State {
  private:
    bool state = false;
    byte delta = 0;

  public:
    bool getState() const;
    byte getDelta() const;

    void setState(bool);
    void press();
    void release();
    void resetDelta();
  };

  State up, left, down, right;
  State jump, run;
};

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

  StringList args;

  Keys keys;

  TimeInfo ticktime;
  TimeInfo rendertime;

  Music* music = nullptr;
  Sound* sound = nullptr;

  sf::RenderTexture* viewport = nullptr;
  Vec2<uint> fbsize;
  uint fbscale = 1;

private:
  bool deinitPhysFS = false;

  sf::RenderWindow* window = nullptr;

  std::vector<Event> events;
  std::vector<GameState*> states;

protected:
  void onKeyEvent(sf::Event&);
  void onResize(Vec2<uint>);

public:
  void tickKeys();

  bool init();
  bool setupPhysFS(std::string, std::string, std::string);

  void pushState(GameState::Factory);
  GameState* popState();

  void handleEvents();
  void update();
  void draw();

  int main();

  Engine(const StringList&);
  ~Engine();
};
}
