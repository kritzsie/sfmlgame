#include "fmt/format.h"
#include "geometry.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <stdexcept>

#include <cstddef>
#include <ctgmath>

namespace geometry {
template<typename T>
T& Vector2<T>::operator[](size_t index) const {
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
Vector2<T> Vector2<T>::operator+() const {
  return Vector2<T>(+x, +y);
}

template<typename T>
Vector2<T> Vector2<T>::operator-() const {
  return Vector2<T>(-x, -y);
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& v) const {
  return Vector2<T>(x + v.x, y + v.y);
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& v) const {
  return Vector2<T>(x - v.x, y - v.y);
}

template<typename T>
Vector2<T> Vector2<T>::operator*(const T n) const {
  return Vector2<T>(x * n, y * n);
}

template<typename T>
Vector2<T> operator*(T n, const Vector2<T>& v) {
  return Vector2<T>(n * v.x, n * v.y);
}

template<typename T>
Vector2<T> Vector2<T>::operator/(T n) const {
  return Vector2<T>(x / n, y / n);
}

template<typename T>
Vector2<T> operator/(T n, const Vector2<T>& v) {
  return Vector2<T>(n / v.x, n / v.y);
}

template<typename T>
bool Vector2<T>::operator==(const Vector2<T>& v) const {
  return (x == v.x) and (y == v.y);
}

template<typename T>
bool Vector2<T>::operator!=(const Vector2<T>& v) const {
  return (x != v.x) and (y != v.y);
}

template<typename T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& v) {
  x = v.x;
  y = v.y;
  return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& v) {
  x += v.x;
  y += v.y;
  return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator*=(T n) {
  x *= n;
  y *= n;
  return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator/=(T n) {
  x /= n;
  y /= n;
  return *this;
}

template<typename T>
T Vector2<T>::dot(const Vector2<T>& v) const {
  return acos(x * v.x + y * v.y);
}

template<typename T>
T Vector2<T>::length() const {
  return sqrt(x * x + y * y);
}

template<typename T>
Vector2<T> Vector2<T>::normalized() const {
  return *this / length();
}

template<typename T>
Vector2<T> Vector2<T>::apply(T (&f)(T)) {
  return Vector2<T>(f(x), f(y));
}

template<typename T>
Vector2<T> Vector2<T>::apply(T (&f)(T, T), T n) {
  return Vector2<T>(f(x, n), f(y, n));
}

template<typename T> template<typename U>
Vector2<T>::operator sf::Vector2<U>() const {
  return sf::Vector2<U>(x, y);
}

template<typename T>
Vector2<T>::Vector2() : x(), y() {}

template<typename T>
Vector2<T>::Vector2(T n) : x(n), y(n) {}

template<typename T>
Vector2<T>::Vector2(T x, T y) : x(x), y(y) {}

template<typename T>
Vector2<T>::Vector2(const Vector2<T>& v) : x(v.x), y(v.y) {}

template<typename T> template<typename U>
Vector2<T>::Vector2(const sf::Vector2<U>& v) : Vector2<T>(v.x, v.y) {}

template<typename T>
Rect<T> Rect<T>::operator+() const {
  return Rect(+pos, +size);
}

template<typename T>
Rect<T> Rect<T>::operator-() const {
  return Rect(-pos, -size);
}

template<typename T>
Rect<T> Rect<T>::operator+(const Rect<T>& r) const {
  return Rect(pos + r.pos, size + r.size);
}

template<typename T>
Rect<T> Rect<T>::operator-(const Rect<T>& r) const {
  return Rect(pos - r.pos, size - r.size);
}

template<typename T>
Rect<T> Rect<T>::operator*(T n) const {
  return Rect(pos * n, size * n);
}

template<typename T>
Rect<T> operator*(T n, const Rect<T>& r) {
  return Rect(n / r.pos, n / r.size);
}

template<typename T>
Rect<T> Rect<T>::operator/(T n) const {
  return Rect(pos / n, size / n);
}

template<typename T>
Rect<T> operator/(T n, const Rect<T>& r) {
  return Rect(n / r.pos, n / r.size);
}

template<typename T>
bool Rect<T>::operator==(const Rect<T>& v) const {
  return (this->pos == v.pos) and (this->size == v.size);
}

template<typename T>
bool Rect<T>::operator!=(const Rect<T>& v) const {
  return (this->pos != v.pos) and (this->size != v.size);
}

template<typename T>
Rect<T>& Rect<T>::operator=(const Rect<T>& r) {
  pos = r.pos;
  size = r.size;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator+=(const Rect<T>& r) {
  pos += r.pos;
  size += r.size;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator-=(const Rect<T>& r) {
  pos -= r.pos;
  size -= r.size;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator*=(T n) {
  pos *= n;
  size *= n;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator/=(T n) {
  pos /= n;
  size /= n;
  return *this;
}

template<typename T> template<typename U>
Rect<T>::operator sf::Rect<U>() const {
  return sf::Rect<U>(pos, size);
}

template<typename T>
Rect<T>::Rect() : pos(), size() {}

template<typename T>
Rect<T>::Rect(T n, T s) : pos(n), size(s) {}

template<typename T>
Rect<T>::Rect(T x, T y, T w, T h) : pos(x, y), size(w, h) {}

template<typename T>
Rect<T>::Rect(const Vector2<T>& v, T w, T h) : pos(v), size(w, h) {}

template<typename T>
Rect<T>::Rect(T x, T y, const Vector2<T>& v) : pos(x, y), size(v) {}

template<typename T>
Rect<T>::Rect(const Vector2<T>& v, const Vector2<T>& u) : pos(v), size(u) {}

template<typename T> template<typename U>
Rect<T>::Rect(const Rect<U>& r) : pos(r.pos), size(r.size) {}

template<typename T>
T& Matrix<T>::Proxy::operator[](int y) const {
  if (y < size.y)
    return data[x + y * size.x];
  else
    throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
}

template<typename T>
Matrix<T>::Proxy::Proxy(const T* const data, const Vector2<size_t>& size, int x) : data(data), size(size), x(x) {}

template<typename T>
typename Matrix<T>::Proxy Matrix<T>::operator[](int x) {
  if (x < size.x)
    return Proxy(data, size, x);
  else
    throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

template<typename T>
Matrix<T>::Matrix(size_t x, size_t y) : size(x, y), data(new T[x * y]()) {}

template<typename T>
Matrix<T>::~Matrix() {
  delete data;
}
}
