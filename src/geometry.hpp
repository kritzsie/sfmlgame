#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <initializer_list>

#include <cstddef>

namespace geometry {
  template<typename T = float>
  class Vector2 {
  public:
    T x, y;

    T& operator[](const size_t) const;
    Vector2<T> operator+() const;
    template<typename U>
    Vector2<T> operator+(const Vector2<U>&) const;
    Vector2<T> operator-() const;
    template<typename U>
    Vector2<T> operator-(const Vector2<U>&) const;
    template<typename U>
    Vector2<T> operator*(U) const;
    template<typename U>
    Vector2<T> operator/(U) const;
    Vector2<T>& operator=(const std::initializer_list<T>&);
    template<typename U>
    Vector2<T>& operator=(const Vector2<U>&);
    template<typename U>
    T dot(const Vector2<U>&) const;
    Vector2();
    Vector2(const T);
    Vector2(const T, const T);
    Vector2(const std::initializer_list<T>&);
    template<typename U>
    Vector2(const Vector2<U>&);
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

#endif
