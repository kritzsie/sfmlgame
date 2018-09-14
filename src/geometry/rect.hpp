#pragma once

#include "vec2.hpp"

#include <SFML/Graphics.hpp>

namespace ke {
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

  operator bool() const;
  operator sf::Rect<T>() const;

  Vec2<T> center() const;
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
