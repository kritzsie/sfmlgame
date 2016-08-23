#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

namespace geometry {
  template <typename TNumber>
  class Vector2 {
  public:
    TNumber x = 0.0;
    TNumber y = 0.0;

    Vector2();
    Vector2(const TNumber x);
    Vector2(const TNumber x, const TNumber y);
    Vector2(const Vector2&);
    ~Vector2();

    TNumber arg() const;
    TNumber abs() const;
  };
}

#endif
