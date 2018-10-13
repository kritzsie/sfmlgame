#pragma once

#include "assetmanager.hpp"
#include "geometry.hpp"
#include "input.hpp"
#include "music.hpp"
#include "renderer.hpp"
#include "sound.hpp"
#include "states.hpp"
#include "tiletypes.hpp"
#include "types.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace ke {
using Clock = std::chrono::steady_clock;

class TimeInfo {
public:
  float rate;
  float delta;

  TimeInfo();
  TimeInfo(float);
};

class TileRedefinitionError : public std::logic_error {
public:
  TileRedefinitionError(const std::string&);
  TileRedefinitionError(const char*);
};

class Engine {
public:
  enum class StateEventType {
    pushState, popState
  };

  using StateEvent = std::pair<StateEventType, BaseState::Factory>;

private:
  bool deinitPhysFS = false;

  sf::RenderWindow* window = nullptr;

  std::vector<StateEvent> events;
  std::vector<BaseState*> states;

  // TODO: move tiledef functions to a more suitable location
  std::map<tileid_t, TileDef> tiles;

public:
  StringList args;

  TimeInfo ticktime;
  TimeInfo rendertime;

  Music* music = nullptr;
  Sound* sound = nullptr;

  sf::RenderTexture* viewport = nullptr;
  Vec2<uint> viewsize;
  uint viewscale;

  InputMap inputs;

protected:
  void onKeyEvent(sf::Event&);
  void onResize(Vec2<uint>);

public:
  bool setupPhysFS(std::string, std::string, std::string);

  void pushState(BaseState::Factory);
  BaseState* popState();

  void registerTileDef(tileid_t, TileDef);
  const TileDef& getTileDef(tileid_t) const;

  void handleEvents();
  void update();
  void draw();

  bool init();
  int main();

  Engine(const StringList&);
  ~Engine();
};
}
