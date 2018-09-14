#include "matrix.hpp"

#include <cstddef>
#include <sstream>

namespace ke {
template<typename T>
T& Matrix<T>::Proxy::operator[](int y) {
  if (y < size.y) {
    return data[x + y * size.x];
  }
  else {
    std::stringstream ss;
    ss << "y index out of bounds (" << y << " >= " << size.y << ")";
    throw ss.str();
  }
}

template<typename T>
Matrix<T>::operator bool() {
  for (int x = 0; x < size.x; x++)
  for (int y = 0; y < size.y; y++) {
    if (static_cast<bool>((*this)[x][y])) {
      return true;
    }
  }
  return false;
}

template<typename T>
Matrix<T>::Proxy::Proxy(T* const data, const Vec2i& size, int x)
: data(data), size(size), x(x) {}

template<typename T>
typename Matrix<T>::Proxy Matrix<T>::operator[](int x) {
  if (x < size.x) {
    return Proxy(data, size, x);
  }
  else {
    std::stringstream ss;
    ss << "x index out of bounds (" << x << " >= " << size.x << ")";
    throw ss.str();
  }
}

template<typename T>
Matrix<T>::Matrix(Vec2i size) : size(size), data(new T[size.x * size.y]()) {}

template<typename T>
Matrix<T>::Matrix(int x, int y) : Matrix(Vec2i(x, y)) {}

template<typename T>
Matrix<T>::~Matrix() {
  delete data;
}
}
