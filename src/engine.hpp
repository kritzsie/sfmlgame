#pragma once

#include "assetmanager.hpp"
#include "geometry.hpp"
#include "music.hpp"
#include "sound.hpp"
#include "types.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <map>
#include <string>
#include <vector>

#include <cstddef>
#include <cstdint>

namespace ke {
using Vec2f = Vec2<float>;

using arglist = std::vector<std::string>;

class Engine {
private:
  static std::size_t instance_count;

protected:
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
  } keys;

  arglist args;
  uint tick;
  sf::Clock tickClock;
  float tickTime, tickRate;
  sf::Event event;
  sf::RenderWindow* window;
  GFXAssetManager gfxassets;
  SFXAssetManager sfxassets;
  Music* music;
  Sound* sound;
  World* world;

  void resize(Vec2<std::size_t>);
  void onKeyEvent();
  void tickKeys();
  void doTick();
  void drawBG(uint32_t);
  void drawBG(std::string, Vec2f);
  void drawBGBottom(std::string, Vec2f);
  void drawBGTop(std::string, Vec2f);
  void drawTiles();
  void drawEntities();
  void drawUI();
  void doRender();
  bool setupPhysFS();
  bool init();

public:
  int exec();
  Engine(const arglist&);
  ~Engine();
};
}
