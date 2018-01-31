#include "../fmt/format.h"
#include "vec2.hpp"

#include <SFML/Graphics.hpp>

#include <stdexcept>

#include <cstddef>
#include <ctgmath>

namespace geometry::vec2 {
template<typename T>
T& Vec2<T>::operator[](size_t index) const {
  switch (index) {
  case 0:
    return x;
  case 1:
    return y;
  default:
    throw std::out_of_range(fmt::format("array index out of bounds ({} >= 2)", index));
  }
}

template<typename T>
Vec2<T> Vec2<T>::operator+() const {
  return Vec2<T>(+x, +y);
}

template<typename T>
Vec2<T> Vec2<T>::operator-() const {
  return Vec2<T>(-x, -y);
}

template<typename T>
Vec2<T> Vec2<T>::operator+(const Vec2<T>& v) const {
  return Vec2<T>(x + v.x, y + v.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator-(const Vec2<T>& v) const {
  return Vec2<T>(x - v.x, y - v.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator*(const T n) const {
  return Vec2<T>(x * n, y * n);
}

template<typename T>
Vec2<T> operator*(T n, const Vec2<T>& v) {
  return Vec2<T>(n * v.x, n * v.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator/(T n) const {
  return Vec2<T>(x / n, y / n);
}

template<typename T>
Vec2<T> operator/(T n, const Vec2<T>& v) {
  return Vec2<T>(n / v.x, n / v.y);
}

template<typename T>
bool Vec2<T>::operator==(const Vec2<T>& v) const {
  return (x == v.x) and (y == v.y);
}

template<typename T>
bool Vec2<T>::operator!=(const Vec2<T>& v) const {
  return (x != v.x) and (y != v.y);
}

template<typename T>
Vec2<T>& Vec2<T>::operator=(const Vec2<T>& v) {
  x = v.x;
  y = v.y;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& v) {
  x += v.x;
  y += v.y;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator*=(T n) {
  x *= n;
  y *= n;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator/=(T n) {
  x /= n;
  y /= n;
  return *this;
}

template<typename T>
T Vec2<T>::dot(const Vec2<T>& v) const {
  return acos(x * v.x + y * v.y);
}

template<typename T>
T Vec2<T>::length() const {
  return sqrt(x * x + y * y);
}

template<typename T>
Vec2<T> Vec2<T>::normalized() const {
  return *this / length();
}

template<typename T>
Vec2<T> Vec2<T>::apply(T (&f)(T)) {
  return Vec2<T>(f(x), f(y));
}

template<typename T>
Vec2<T> Vec2<T>::apply(T (&f)(T, T), T n) {
  return Vec2<T>(f(x, n), f(y, n));
}

template<typename T> template<typename U>
Vec2<T>::operator sf::Vector2<U>() const {
  return sf::Vector2<U>(x, y);
}

template<typename T>
Vec2<T>::Vec2() : x(), y() {}

template<typename T>
Vec2<T>::Vec2(T n) : x(n), y(n) {}

template<typename T>
Vec2<T>::Vec2(T x, T y) : x(x), y(y) {}

template<typename T>
Vec2<T>::Vec2(const Vec2<T>& v) : x(v.x), y(v.y) {}

template<typename T> template<typename U>
Vec2<T>::Vec2(const sf::Vector2<U>& v) : Vec2<T>(v.x, v.y) {}
}
