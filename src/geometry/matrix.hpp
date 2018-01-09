#ifndef GEOMETRY_MATRIX_HPP
#define GEOMETRY_MATRIX_HPP

#include "vector2.hpp"

#include <cstddef>

namespace geometry::matrix {
using geometry::vector2::Vector2;

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
  using namespace geometry::matrix;
}

#endif // GEOMETRY_MATRIX_HPP
