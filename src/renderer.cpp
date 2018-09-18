#include "renderer.hpp"

namespace ke {
void RenderStates::pushFrame(std::string state_arg, RenderFrame renderframe) {
  states.at(state_arg).push_back(renderframe);
}

void RenderStates::pushFrame(std::string state_arg, std::string texture,
                             Rect<int> cliprect, Vec2f offset_arg, float duration) {
  states[state_arg].push_back(
    RenderFrame{texture, cliprect, offset_arg, duration}
  );
}

void RenderStates::pushFrame(std::string state_arg, std::string texture,
                             Vec2i origin, Vec2f offset_arg, float duration) {
  pushFrame(state_arg, texture, Rect<int>(origin, Vec2(16)), offset_arg, duration);
}

// WARNING: slow. slooooooooow...
StringList RenderStates::getStateList() const {
  StringList state_list;
  for (auto& it : states) {
    state_list.push_back(it.first);
  }
  return state_list;
}

std::size_t RenderStates::getFrameCount(std::string state_arg) const {
  return states.at(state_arg).size();
}

void RenderStates::setState(std::string state_arg, std::size_t offset_arg) {
  state = state_arg;
  offset = offset_arg;
}

const std::string& RenderStates::getState() const {
  return state;
}

const RenderFrame& RenderStates::getFrame() const {
  return states.at(state).at(offset);
}

RenderStates::RenderStates() : state("idle") {}
}
