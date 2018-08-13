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
using ArgList = std::vector<std::string>;

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

  ArgList args;

  Keys keys;

  TimeInfo ticktime;
  TimeInfo rendertime;

  GFXAssetManager gfxassets;
  SFXAssetManager sfxassets;

  sf::RenderTexture* viewport = nullptr;
  sf::RenderWindow* window = nullptr;

  Music* music = nullptr;
  Sound* sound = nullptr;
  World* world = nullptr;

  std::vector<GameState*> gamestates;

protected:
  void onKeyEvent(sf::Event&);
  void onResize(Vec2<std::size_t>);

  void tickKeys();

public:
  void doTick();
  void doRender();

  void drawBG(uint32_t);
  void drawBG(std::string, Vec2f);
  void drawBGBottom(std::string, Vec2f);
  void drawBGTop(std::string, Vec2f);
  void drawTiles();
  void drawEntities();
  void drawUI();

  bool init();
  bool setupPhysFS(std::string, std::string, std::string);

  void handleEvents();
  void update();
  void draw();
  bool loop();

  int main();

  Engine(const ArgList&);
  ~Engine();
};
}
