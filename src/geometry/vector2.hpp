#ifndef GEOMETRY_VECTOR2_HPP
#define GEOMETRY_VECTOR2_HPP

#include <SFML/Graphics.hpp>

#include <cstddef>

namespace geometry::vector2 {
template<typename T = double>
class Vector2 {
public:
  T x, y;

  T& operator[](const size_t) const;

  Vector2<T> operator+() const;
  Vector2<T> operator-() const;
  Vector2<T> operator+(const Vector2<T>&) const;
  Vector2<T> operator-(const Vector2<T>&) const;
  Vector2<T> operator*(T) const;
  Vector2<T> operator/(T) const;

  bool operator==(const Vector2<T>&) const;
  bool operator!=(const Vector2<T>&) const;

  Vector2<T>& operator=(const Vector2<T>&);
  Vector2<T>& operator+=(const Vector2<T>&);
  Vector2<T>& operator-=(const Vector2<T>&);
  Vector2<T>& operator*=(T);
  Vector2<T>& operator/=(T);

  T dot(const Vector2<T>&) const;
  T length() const;
  Vector2 normalized() const;

  Vector2 apply(T (&)(T));
  Vector2 apply(T (&)(T, T), T);

  template<typename U>
  operator sf::Vector2<U>() const;

  Vector2();
  Vector2(T);
  Vector2(T, T);
  Vector2(const Vector2<T>&);
  template<typename U>
  Vector2(const sf::Vector2<U>&);
};
}

namespace keng {
  using namespace geometry::vector2;
}

#include "vector2.cpp"

#endif // GEOMETRY_VECTOR2_HPP
