#pragma once

#include <cmath>

namespace ke::util {
template<typename T>
constexpr T absmod(T x, T y) {
  return ((x % y) + y) % y;
}

template<>
constexpr float absmod<float>(float x, float y) {
  return std::fmod(std::fmod(x, y) + y, y);
}

template<>
constexpr double absmod<double>(double x, double y) {
  return std::fmod(std::fmod(x, y) + y, y);
}

template<>
constexpr long double absmod<long double>(long double x, long double y) {
  return std::fmod(std::fmod(x, y) + y, y);
}
}
