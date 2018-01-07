#include "fmt/format.h"
#include "geometry.hpp"

#include <stdexcept>

#include <cstddef>
#include <ctgmath>

namespace geometry {
template<typename T>
T& Vector2<T>::operator[](const size_t index) const {
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
Vector2<T> operator*(const T n, const Vector2<T>& v) {
  return Vector2<T>(n * v.x, n * v.y);
}

template<typename T>
Vector2<T> Vector2<T>::operator/(const T n) const {
  return Vector2<T>(x / n, y / n);
}

template<typename T>
Vector2<T> operator/(const T n, const Vector2<T>& v) {
  return Vector2<T>(n / v.x, n / v.y);
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
Vector2<T>& Vector2<T>::operator*=(const T n) {
  x *= n;
  y *= n;
  return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator/=(const T n) {
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
Vector2<T> Vector2<T>::apply(const T (&f)(const T)) {
  return Vector2<T>(f(x), f(y));
}

template<typename T>
Vector2<T> Vector2<T>::apply(const T (&f)(const T, const T), const T n) {
  return Vector2<T>(f(x, n), f(y, n));
}

template<typename T> template<typename U>
Vector2<T>::operator sf::Vector2<U>() const {
  return sf::Vector2<U>(x, y);
}

template<typename T>
Vector2<T>::Vector2() : x(), y() {}

template<typename T>
Vector2<T>::Vector2(const T n) : x(n), y(n) {}

template<typename T>
Vector2<T>::Vector2(const T x, const T y) : x(x), y(y) {}

template<typename T>
Vector2<T>::Vector2(const Vector2<T>& v) : x(v.x), y(v.y) {}

template<typename T> template<typename U>
Vector2<T>::Vector2(const sf::Vector2<U>& v) : Vector2<T>(v.x, v.y) {}

template<typename T>
T& Matrix<T>::Proxy::operator[](const int y) const {
  if (y < size.y)
    return data[x + y * size.x];
  else
    throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
}

template<typename T>
Matrix<T>::Proxy::Proxy(T* const data, const Vector2<size_t>& size, const int x) : data(data), size(size), x(x) {}

template<typename T>
typename Matrix<T>::Proxy Matrix<T>::operator[](const int x) {
  if (x < size.x)
    return Proxy(data, size, x);
  else
    throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

template<typename T>
Matrix<T>::Matrix(const size_t x, const size_t y) : size(x, y), data(new T[x * y]()) {}

template<typename T>
Matrix<T>::~Matrix() {
  delete data;
}
}
