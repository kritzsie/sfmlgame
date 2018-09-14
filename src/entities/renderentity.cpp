#include "renderentity.hpp"

#include "../engine.hpp"
#include "../types.hpp"

#include <cmath>
#include <cstddef>

namespace ke {
byte Sprite::getDirection() const {
  return direction;
}

void Sprite::setDirection(byte d) {
  direction = std::copysign(1, d);
}

Sprite::Sprite() : RenderStates(), scale(1.f) {}

void RenderStates::pushFrame(std::string state_arg, RenderFrame renderframe) {
  frames.at(state_arg).push_back(renderframe);
}

void RenderStates::pushFrame(std::string state_arg, std::string texture,
                             Rect<int> cliprect, Vec2f offset_arg, float duration) {
  frames[state_arg].push_back(RenderFrame{texture, cliprect, offset_arg, duration});
}

// WARNING: slow. slooooooooow...
StringList RenderStates::getStateList() const {
  StringList state_list;
  for (auto& it : frames) {
    state_list.push_back(it.first);
  }
  return state_list;
}

std::size_t RenderStates::getFrameCount(std::string state_arg) const {
  return frames.at(state_arg).size();
}

void RenderStates::setState(std::string state_arg, std::size_t offset_arg) {
  state = state_arg;
  offset = offset_arg;
}

const std::string& RenderStates::getState() const {
  return state;
}

const RenderFrame& RenderStates::getFrame() const {
  return frames.at(state).at(offset);
}

RenderStates::RenderStates() {}

RenderEntity::Factory RenderEntity::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new RenderEntity(engine, world);
  };
}

void RenderEntity::update() {
  time += engine->ticktime.rate;
}

RenderEntity::RenderEntity(Engine* engine, World* world)
: BaseEntity(engine, world), Sprite() {}
}
