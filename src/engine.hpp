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

using arglist_t = std::vector<std::string>;

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
  float delta = 0;

  TimeInfo();
  TimeInfo(float);
};

class Engine {
private:
  bool deinitPhysFS = false;

  arglist_t args;

  Keys keys;

  TimeInfo ticktime;
  TimeInfo rendertime;

  GFXAssetManager gfxassets;
  SFXAssetManager sfxassets;

  sf::RenderTexture viewport;
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

  void update();
  void draw();

  int main();

  Engine(const arglist_t&);
  ~Engine();
};
}
