#include "vec2.hpp"

#include <SFML/Graphics.hpp>

#include <stdexcept>

#include <cstddef>
#include <ctgmath>

namespace geometry::vec2 {
template<typename T>
Vec2<T> Vec2<T>::operator+() const {
  return Vec2<T>(+x, +y);
}

template<typename T>
Vec2<T> Vec2<T>::operator+(const Vec2<T>& rhs) const {
  return Vec2<T>(x + rhs.x, y + rhs.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator-() const {
  return Vec2<T>(-x, -y);
}

template<typename T>
Vec2<T> Vec2<T>::operator-(const Vec2<T>& rhs) const {
  return Vec2<T>(x - rhs.x, y - rhs.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator*(const T rhs) const {
  return Vec2<T>(x * rhs, y * rhs);
}

template<typename T>
Vec2<T> operator*(T lhs, const Vec2<T>& rhs) {
  return Vec2<T>(lhs * rhs.x, lhs * rhs.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator/(T rhs) const {
  return Vec2<T>(x / rhs, y / rhs);
}

template<typename T>
Vec2<T> operator/(T lhs, const Vec2<T>& rhs) {
  return Vec2<T>(lhs / rhs.x, lhs / rhs.y);
}

template<typename T>
bool Vec2<T>::operator==(const Vec2<T>& rhs) const {
  return (x == rhs.x) and (y == rhs.y);
}

template<typename T>
bool Vec2<T>::operator!=(const Vec2<T>& rhs) const {
  return (x != rhs.x) and (y != rhs.y);
}

template<typename T>
Vec2<T>& Vec2<T>::operator=(const Vec2<T>& rhs) {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator*=(T rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator/=(T rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
}

template<typename T>
T Vec2<T>::dot(const Vec2<T>& rhs) const {
  return acos(x * rhs.x + y * rhs.y);
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
Vec2<T> Vec2<T>::map(T (&f)(T)) {
  return Vec2<T>(f(x), f(y));
}

template<typename T>
Vec2<T> Vec2<T>::map(T (&f)(T, T), T rhs) {
  return Vec2<T>(f(x, rhs), f(y, rhs));
}

template<typename T>
Vec2<T>::operator sf::Vector2<T>() const {
  return sf::Vector2<T>(x, y);
}

template<typename T>
Vec2<T>::Vec2() : x(), y() {}

template<typename T>
Vec2<T>::Vec2(T both) : x(both), y(both) {}

template<typename T>
Vec2<T>::Vec2(T x, T y) : x(x), y(y) {}

template<typename T>
Vec2<T>::Vec2(const Vec2<T>& vec2) : x(vec2.x), y(vec2.y) {}

template<typename T>
Vec2<T>::Vec2(const sf::Vector2<T>& vec2) : Vec2<T>(vec2.x, vec2.y) {}
}
