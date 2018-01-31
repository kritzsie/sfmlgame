#ifndef GEOMETRY_RECT_HPP
#define GEOMETRY_RECT_HPP

#include "vec2.hpp"

#include <SFML/Graphics.hpp>

namespace geometry::rect {
using geometry::vec2::Vec2;

template<typename T>
class Rect {
public:
  Vec2<T> pos;
  Vec2<T> size;

  T& x = pos.x;
  T& y = pos.y;
  T& w = size.x;
  T& h = size.y;

  Rect<T> operator+() const;
  Rect<T> operator-() const;
  Rect<T> operator+(const Rect<T>&) const;
  Rect<T> operator-(const Rect<T>&) const;
  Rect<T> operator*(T) const;
  Rect<T> operator/(T) const;

  bool operator==(const Rect<T>&) const;
  bool operator!=(const Rect<T>&) const;

  Rect<T>& operator=(const Rect<T>&);
  Rect<T>& operator+=(const Rect<T>&);
  Rect<T>& operator-=(const Rect<T>&);
  Rect<T>& operator*=(T);
  Rect<T>& operator/=(T);

  operator sf::Rect<T>() const;

  bool intersects(const Rect<T>&) const;
  Rect<T> intersection(const Rect<T>&) const;

  Rect();
  Rect(T);
  Rect(T, T, T, T);
  Rect(const Rect<T>&);
  Rect(const Vec2<T>&, const Vec2<T>&);
};
}

#include "rect.cpp"

namespace keng {
  using namespace geometry::rect;
}

#endif // GEOMETRY_RECT_HPP
