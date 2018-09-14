#include "camera.hpp"

#include "../engine.hpp"

namespace ke {
Camera::Factory Camera::create() {
  return [](Engine* engine, World* world) -> BaseEntity* {
    return new Camera(engine, world);
  };
}

BaseEntity* Camera::getTarget() const {
  return target;
}

void Camera::setTarget(BaseEntity* entity) {
  target = entity;
}

void Camera::update() {
  if (target != nullptr) {
    float y_offset = target->pos.y;
    if (target == world->player) {
      y_offset += world->player->getBBox().y / 2;
    }
    vel = (target->pos + Vec2(0.f, y_offset) - pos) * engine->ticktime.rate / 8.f;
    pos += vel / engine->ticktime.rate;

    const Vec2f view_center(
      engine->viewport->getSize().x / 2.f / engine->viewscale,
      engine->viewport->getSize().y / 2.f / engine->viewscale
    );
    const Padding<float> camera_padding{
      .left = view_center.x + world->padding.left * 16.f,
      .right = world->size.x * 16.f - view_center.x - world->padding.right * 16.f,
      .bottom = view_center.y + world->padding.bottom * 16,
      .top = world->size.y * 16.f - view_center.y - world->padding.top * 16.f
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
}

Camera::Camera(Engine* engine, World* world) : BaseEntity(engine, world) {}
}
