#pragma once

#include "vec2.hpp"
#include "../types.hpp"

namespace ke {
template<typename T>
class Matrix {
protected:
  class Proxy {
  private:
    T* const data;
    const Vec2i& size;
    const int x;

  public:
    T& operator[](int);

    Proxy(T* const, const Vec2i&, int);
  };

public:
  const Vec2i size;

private:
  T* data;

public:
  Proxy operator[](int);

  operator bool();

  Matrix(Vec2i);
  Matrix(int, int);
  ~Matrix();
};
}

#include "matrix.cpp"
