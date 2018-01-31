#ifndef GEOMETRY_VEC2_HPP
#define GEOMETRY_VEC2_HPP

#include <SFML/Graphics.hpp>

#include <cstddef>

namespace geometry::vec2 {
template<typename T>
class Vec2 {
public:
  T x, y;

  T& operator[](const size_t) const;

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

  Vec2 apply(T (&)(T));
  Vec2 apply(T (&)(T, T), T);

  template<typename U>
  operator sf::Vector2<U>() const;

  Vec2();
  Vec2(T);
  Vec2(T, T);
  Vec2(const Vec2<T>&);
  template<typename U>
  Vec2(const sf::Vector2<U>&);
};
}

#include "vec2.cpp"

namespace keng {
  using namespace geometry::vec2;
}

#endif // GEOMETRY_VEC2_HPP
