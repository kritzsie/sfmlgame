#pragma once

#include <SFML/Graphics.hpp>

namespace ke {
template<typename T>
class Vec2 {
public:
  T x, y;

  Vec2<T> operator+() const;
  Vec2<T> operator-() const;
  Vec2<T> operator+(const Vec2<T>&) const;
  Vec2<T> operator-(const Vec2<T>&) const;
  Vec2<T> operator*(T) const;
  Vec2<T> operator/(T) const;

  bool operator==(const Vec2<T>&) const;
  bool operator!=(const Vec2<T>&) const;

  Vec2<T>& operator=(const Vec2<T>&);
  Vec2<T>& operator+=(const Vec2<T>&);
  Vec2<T>& operator-=(const Vec2<T>&);
  Vec2<T>& operator*=(T);
  Vec2<T>& operator/=(T);

  T dot(const Vec2<T>&) const;
  T length() const;
  Vec2 normalized() const;

  Vec2 map(T (&)(T));
  Vec2 map(T (&)(T, T), T);

  operator bool() const;
  operator sf::Vector2<T>() const;

  Vec2();
  Vec2(T);
  Vec2(T, T);
  Vec2(const Vec2<T>&);
  Vec2(const sf::Vector2<T>&);
};
}

#include "vec2.cpp"
