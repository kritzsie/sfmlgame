#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <SFML/Graphics.hpp>

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
  Vector2<T> operator*(T) const;
  Vector2<T> operator/(T) const;

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

template<typename T = double>
class Rect {
public:
  Vector2<T> pos;
  Vector2<T> size;
  T& x = pos.x;
  T& y = pos.y;
  T& w = size.x;
  T& h = size.y;

  template<typename U>
  operator sf::Rect<U>() const;

  Rect();
  Rect(T, T);
  Rect(T, T, T, T);
  Rect(const Vector2<T>&, T, T);
  Rect(T, T, const Vector2<T>&);
  Rect(const Vector2<T>&, const Vector2<T>&);
  template<typename U>
  Rect(const Rect<U>&);
};

template<typename T = double>
class Matrix {
protected:
  class Proxy {
  private:
    const int x;
    const T* const data;
    const Vector2<size_t>& size;

  public:
    T& operator[](int) const;

    Proxy(const T* const, const Vector2<size_t>&, int);
  };

  T* data;

public:
  const Vector2<size_t> size;

  Proxy operator[](int);
  Matrix(size_t, size_t);
  ~Matrix();
};
}

namespace keng {
  using namespace geometry;
}

#include "geometry.tpp"

#endif
