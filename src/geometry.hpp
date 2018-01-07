#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <SFML/System.hpp>

#include <cstddef>

namespace geometry {
template<typename T = double>
class Vector2 {
public:
  T x, y;

  T& operator[](const size_t) const;
  Vector2<T> operator+() const;
  Vector2<T> operator-() const;

  Vector2<T> operator+(const Vector2<T>&) const;
  Vector2<T> operator-(const Vector2<T>&) const;
  Vector2<T> operator*(const T) const;
  Vector2<T> operator/(const T) const;

  Vector2<T>& operator=(const Vector2<T>&);

  Vector2<T>& operator+=(const Vector2<T>&);
  Vector2<T>& operator-=(const Vector2<T>&);
  Vector2<T>& operator*=(const T);
  Vector2<T>& operator/=(const T);

  T dot(const Vector2<T>&) const;
  T length() const;
  Vector2 normalized() const;

  Vector2 apply(const T (&)(const T));
  Vector2 apply(const T (&)(const T, const T), const T);

  template<typename U>
  operator sf::Vector2<U>() const;

  Vector2();
  Vector2(const T);
  Vector2(const T, const T);
  Vector2(const Vector2<T>&);
  template<typename U>
  Vector2(const sf::Vector2<U>&);
};

template<typename T = float>
class Matrix {
private:
  class Proxy {
  private:
    const int x;
    T* const data;

  public:
    const Vector2<size_t>& size;

    T& operator[](const int) const;

    Proxy(T* const, const Vector2<size_t>&, const int);
  };

  T* data;

public:
  const Vector2<size_t> size;

  Proxy operator[](const int);
  Matrix(const size_t, const size_t);
  ~Matrix();
};
}

namespace keng {
  using namespace geometry;
}

#include "geometry.tpp"

#endif
