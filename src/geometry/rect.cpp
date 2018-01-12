#include "rect.hpp"

#include <SFML/Graphics.hpp>

namespace geometry::rect {
template<typename T>
Rect<T> Rect<T>::operator+() const {
  return Rect(+pos, +size);
}

template<typename T>
Rect<T> Rect<T>::operator-() const {
  return Rect(-pos, -size);
}

template<typename T>
Rect<T> Rect<T>::operator+(const Rect<T>& r) const {
  return Rect(pos + r.pos, size + r.size);
}

template<typename T>
Rect<T> Rect<T>::operator-(const Rect<T>& r) const {
  return Rect(pos - r.pos, size - r.size);
}

template<typename T>
Rect<T> Rect<T>::operator*(T n) const {
  return Rect(pos * n, size * n);
}

template<typename T>
Rect<T> operator*(T n, const Rect<T>& r) {
  return Rect(n / r.pos, n / r.size);
}

template<typename T>
Rect<T> Rect<T>::operator/(T n) const {
  return Rect(pos / n, size / n);
}

template<typename T>
Rect<T> operator/(T n, const Rect<T>& r) {
  return Rect(n / r.pos, n / r.size);
}

template<typename T>
bool Rect<T>::operator==(const Rect<T>& v) const {
  return (this->pos == v.pos) and (this->size == v.size);
}

template<typename T>
bool Rect<T>::operator!=(const Rect<T>& v) const {
  return (this->pos != v.pos) and (this->size != v.size);
}

template<typename T>
Rect<T>& Rect<T>::operator=(const Rect<T>& r) {
  pos = r.pos;
  size = r.size;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator+=(const Rect<T>& r) {
  pos += r.pos;
  size += r.size;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator-=(const Rect<T>& r) {
  pos -= r.pos;
  size -= r.size;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator*=(T n) {
  pos *= n;
  size *= n;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator/=(T n) {
  pos /= n;
  size /= n;
  return *this;
}

template<typename T> template<typename U>
Rect<T>::operator sf::Rect<U>() const {
  return sf::Rect<U>(pos, size);
}

template<typename T>
bool Rect<T>::intersects(const Rect<T>& r) const {
  if (x < r.x + r.w
  and x + w > r.x
  and y < r.y + r.h
  and y + h > r.y)
    return true;
  return false;
}

template<typename T>
Rect<T> Rect<T>::intersection(const Rect<T>& r) const {
  return Rect<T>(
    std::max(x, r.x),
    std::max(y, r.y),
    (x < r.x) ? (x + w - r.x) : (r.x + r.w - x),
    (y < r.y) ? (y + h - r.y) : (r.y + r.h - y)
  );
}

template<typename T>
Rect<T>::Rect() : pos(), size() {}

template<typename T>
Rect<T>::Rect(T n, T s) : pos(n), size(s) {}

template<typename T>
Rect<T>::Rect(T x, T y, T w, T h) : pos(x, y), size(w, h) {}

template<typename T>
Rect<T>::Rect(const Vector2<T>& v, T w, T h) : pos(v), size(w, h) {}

template<typename T>
Rect<T>::Rect(T x, T y, const Vector2<T>& v) : pos(x, y), size(v) {}

template<typename T>
Rect<T>::Rect(const Vector2<T>& v, const Vector2<T>& u) : pos(v), size(u) {}

template<typename T> template<typename U>
Rect<T>::Rect(const Rect<U>& r) : pos(r.pos), size(r.size) {}
}
