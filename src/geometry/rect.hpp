#ifndef GEOMETRY_RECT_HPP
#define GEOMETRY_RECT_HPP

#include <SFML/Graphics.hpp>

namespace geometry::rect {
using keng::Vector2;

template<typename T = double>
class Rect {
public:
  Vector2<T> pos;
  Vector2<T> size;

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

  template<typename U>
  operator sf::Rect<U>() const;

  bool intersects(const Rect<T>&) const;
  Rect<T> intersection(const Rect<T>&) const;

  Rect();
  Rect(T, T);
  Rect(T, T, T, T);
  Rect(const Vector2<T>&, T, T);
  Rect(T, T, const Vector2<T>&);
  Rect(const Vector2<T>&, const Vector2<T>&);
  template<typename U>
  Rect(const Rect<U>&);
};
}

#include "rect.cpp"

namespace keng {
  using namespace geometry::rect;
}

#endif // GEOMETRY_RECT_HPP
