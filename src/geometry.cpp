#include <cstddef>

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
      throw std::out_of_range("array index out of bounds");
    }
  }

  template<typename T>
  Vector2<T>::Vector2(const T x, const T y) : x(x), y(y) {}

  template<typename T>
  T& Matrix<T>::Proxy::operator[](const int y) {
    if (y < size.y) {
      return array[x + y * size.x];
    }
    else {
      throw std::out_of_range("array index out of bounds");
    }
  }

  template<typename T>
  Matrix<T>::Proxy::Proxy(T* const array, const Vector2<size_t>& size, const int x) : array(array), size(size), x(x) {}

  template<typename T>
  inline typename Matrix<T>::Proxy Matrix<T>::operator[](const int x) {
    if (x < size.x) {
      return Proxy(array, size, x);
    }
    else {
      throw std::out_of_range("array index out of bounds");
    }
  }

  template<typename T>
  Matrix<T>::Matrix(const size_t x, const size_t y) : size(x, y), array(new T[x * y]()) {}

  template<typename T>
  Matrix<T>::~Matrix() {
    delete array;
  }
}
