#include <cstddef>

#include "fmt/format.h"
#include "geometry.hpp"

#include <stdexcept>

namespace geometry {
  template<typename T>
  T& Vector2<T>::operator[](const size_t index) {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range(fmt::format("array index out of bounds ({} > 1)", index));
    }
  }

  template<typename T>
  Vector2<T>::Vector2(const T x, const T y) : x(x), y(y) {}

  template<typename T>
  T& Matrix<T>::Proxy::operator[](const int y) {
    if (y < size.y) {
      return data[x + y * size.x];
    }
    else {
      throw std::out_of_range(fmt::format("y index out of bounds ({0} > {1})", y, size.y - 1));
    }
  }

  template<typename T>
  Matrix<T>::Proxy::Proxy(T* const array, const Vector2<size_t>& size, const int x) : data(array), size(size), x(x) {}

  template<typename T>
  inline typename Matrix<T>::Proxy Matrix<T>::operator[](const int x) {
    if (x < size.x) {
      return Proxy(data, size, x);
    }
    else {
      throw std::out_of_range(fmt::format("x index out of bounds ({0} > {1})", x, size.x - 1));
    }
  }

  template<typename T>
  Matrix<T>::Matrix(const size_t x, const size_t y) : size(x, y), data(new T[x * y]()) {}

  template<typename T>
  Matrix<T>::~Matrix() {
    delete data;
  }
}
