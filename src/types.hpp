#pragma once

#include "geometry/rect.hpp"
#include "geometry/vec2.hpp"

#include <map>
#include <string>
#include <vector>

namespace ke {
  using byte = signed char;
  using ubyte = unsigned char;
  using ushort = unsigned short;
  using uint = unsigned int;
  using ulong = unsigned long;

  using FloatRect = Rect<float>;
  using IntRect = Rect<int>;

  using Vec2f = Vec2<float>;
  using Vec2i = Vec2<int>;
  using Vec2u = Vec2<uint>;

  using StringList = std::vector<std::string>;

  template<typename T>
  using StringTable = std::map<std::string, T>;
}
