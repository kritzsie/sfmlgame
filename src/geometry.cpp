#include <ctgmath>

#include "geometry.hpp"

namespace geometry {
  template <typename TNumber>
  Vector2<TNumber>::Vector2() {}

  template <typename TNumber>
  Vector2<TNumber>::Vector2(const TNumber x) : x(x), y(x) {}

  template <typename TNumber>
  Vector2<TNumber>::Vector2(const TNumber x, const TNumber y) : x(x), y(y) {}

  template <typename TNumber>
  Vector2<TNumber>::~Vector2() {}

  template <typename TNumber>
  TNumber Vector2<TNumber>::arg() const {
    return atan2(x, y);
  }
  template <typename TNumber>
  TNumber Vector2<TNumber>::abs() const {
    return hypot(x, y);
  }
}
