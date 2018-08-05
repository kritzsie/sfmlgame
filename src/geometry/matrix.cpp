#include "matrix.hpp"

#include <cstddef>

namespace ke {
template<typename T>
T& Matrix<T>::Proxy::operator[](int y) const {
  if (y < size.y) return data[x + y * size.x];
  else {
    std::stringstream ss;
    ss << "y index out of bounds (" << y << " >= " << size.y << ")";
    throw ss.str();
  }
}

template<typename T>
Matrix<T>::Proxy::Proxy(const T* const data, const Vec2<size_t>& size, int x) : data(data), size(size), x(x) {}

template<typename T>
typename Matrix<T>::Proxy Matrix<T>::operator[](int x) {
  if (x < size.x) return Proxy(data, size, x);
  else {
    std::stringstream ss;
    ss << "x index out of bounds (" << x << " >= " << size.x << ")";
    throw ss.str();
  }
}

template<typename T>
Matrix<T>::Matrix(size_t x, size_t y) : size(x, y), data(new T[x * y]()) {}

template<typename T>
Matrix<T>::~Matrix() {
  delete data;
}
}
