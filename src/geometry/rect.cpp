#include "rect.hpp"

#include <SFML/Graphics.hpp>

namespace ke {
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

template<typename T>
Rect<T>::operator bool() const {
  return static_cast<bool>(pos) and static_cast<bool>(size);
}

template<typename T>
Rect<T>::operator sf::Rect<T>() const {
  return sf::Rect<T>(pos, size);
}

template<typename T>
Vec2<T> Rect<T>::center() const {
  return Vec2(x + w / 2, y + h / 2);
}

template<typename T>
bool Rect<T>::intersects(const Rect<T>& r) const {
  if (x < r.x + r.w
  and x + w > r.x
  and y < r.y + r.h
  and y + h > r.y) {
    return true;
  }
  else {
    return false;
  }
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
Rect<T>::Rect(T both) : pos(both), size(both) {}

template<typename T>
Rect<T>::Rect(T x, T y, T w, T h) : pos(x, y), size(w, h) {}

template<typename T>
Rect<T>::Rect(const Rect<T>& rect) : pos(rect.pos), size(rect.size) {}

template<typename T>
Rect<T>::Rect(const Vec2<T>& pos, const Vec2<T>& size) : pos(pos), size(size) {}
}
