#pragma once

#include <cstdint>

namespace ke {
enum class EntityType : uint32_t {
  None,
  Marker,
  Camera,
  Player,
  Goomba
};
}
