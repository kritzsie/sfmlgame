#include "matrix.hpp"

#include <cstddef>

namespace geometry::matrix {
template<typename T>
T& Matrix<T>::Proxy::operator[](int y) const {
  if (y < size.y)
    return data[x + y * size.x];
  else
    throw std::out_of_range(fmt::format("y index out of bounds ({0} >= {1})", y, size.y));
}

template<typename T>
Matrix<T>::Proxy::Proxy(const T* const data, const Vec2<size_t>& size, int x) : data(data), size(size), x(x) {}

template<typename T>
typename Matrix<T>::Proxy Matrix<T>::operator[](int x) {
  if (x < size.x)
    return Proxy(data, size, x);
  else
    throw std::out_of_range(fmt::format("x index out of bounds ({0} >= {1})", x, size.x));
}

template<typename T>
Matrix<T>::Matrix(size_t x, size_t y) : size(x, y), data(new T[x * y]()) {}

template<typename T>
Matrix<T>::~Matrix() {
  delete data;
}
}
