#include "camera.hpp"

#include "../engine.hpp"
#include "../entities.hpp"

namespace ke {
Camera::Factory Camera::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Camera(engine, world);
  };
}

BaseEntity* Camera::getTarget() const {
  return target;
}

void Camera::setTarget(BaseEntity* new_target) {
  target = new_target;

  Entity* entity = dynamic_cast<Entity*>(new_target);
  if (entity != nullptr) {
    pos = entity->getBBox().center();
  }
  else {
    pos = new_target->pos;
  }
}

void Camera::resolveWorldCollisions() {
  const Vec2f view_center(
    engine->viewport->getSize().x / 2.f / engine->viewscale,
    engine->viewport->getSize().y / 2.f / engine->viewscale
  );
  const Padding<float> camera_padding{
    .left = view_center.x + world->padding.left * 16.f,
    .right = world->getSize().x * 16.f - view_center.x - world->padding.right * 16.f,
    .bottom = view_center.y + world->padding.bottom * 16,
    .top = world->getSize().y * 16.f - view_center.y - world->padding.top * 16.f
  };

  if (pos.x < camera_padding.left
  and pos.x > camera_padding.right) {
    pos.x = (camera_padding.left + camera_padding.right) / 2.f;
  }
  else {
    if (pos.x < camera_padding.left) {
      pos.x = camera_padding.left;
    }
    else if (pos.x > camera_padding.right) {
      pos.x = camera_padding.right;
    }
  }

  if (pos.y > camera_padding.top
  and pos.y < camera_padding.bottom) {
    pos.y = (camera_padding.top + camera_padding.bottom) / 2.f;
  }
  else {
    if (pos.y > camera_padding.top) {
      vel.y = 0.f;
      pos.y = camera_padding.top;
    }
    else if (pos.y < camera_padding.bottom) {
      vel.y = 0.f;
      pos.y = camera_padding.bottom;
    }
  }
}

void Camera::update() {
  if (target != nullptr) {
    Entity* entity = dynamic_cast<Entity*>(target);
    if (entity != nullptr) {
      vel = (entity->getBBox().center() - pos) * engine->ticktime.rate / 8.f;
    }
    else {
      vel = (target->pos - pos) * engine->ticktime.rate / 8.f;
    }
    pos += vel / engine->ticktime.rate;
  }

  resolveWorldCollisions();
}

Camera::Camera(Engine* engine, World* world) : BaseEntity(engine, world) {}
}
