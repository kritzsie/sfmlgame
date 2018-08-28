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
using Vec2f = Vec2<float>;

using Clock = std::chrono::steady_clock;
using StringList = std::vector<std::string>;

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

struct TimeInfo {
public:
  float rate;
  float delta;

  TimeInfo();
  TimeInfo(float);
};

class Engine {
private:
  bool deinitPhysFS = false;

  StringList args;

  sf::RenderWindow* window = nullptr;

  World* world = nullptr;

public:
  Keys keys;

  TimeInfo ticktime;
  TimeInfo rendertime;

  GFXAssetManager gfx;
  SFXAssetManager sfx;

  Music* music = nullptr;
  Sound* sound = nullptr;

  sf::RenderTexture* viewport = nullptr;
  Vec2<uint> fbsize;
  uint fbscale = 1;

  std::vector<GameState*> states;

protected:
  void onKeyEvent(sf::Event&);
  void onResize(Vec2<uint>);

public:
  void tickKeys();

  bool init();
  bool setupPhysFS(std::string, std::string, std::string);

  void handleEvents();
  void update();
  void draw();
  bool loop();

  int main();

  Engine(const StringList&);
  ~Engine();
};
}
