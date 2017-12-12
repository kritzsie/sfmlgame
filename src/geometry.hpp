#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

namespace geometry {
  template<typename T = float>
  class Vector2 {
  public:
    const T x, y;

    T& operator[](const size_t);
    Vector2(const T, const T);
  };

  template<typename T = float>
  class Matrix {
  private:
    class Proxy {
    private:
      const int x;
      T* const data;

    public:
      const Vector2<size_t>& size;

      T& operator[](const int);

      Proxy(T* const, const Vector2<size_t>&, const int);
    };

    T* data;

  public:
    const Vector2<size_t> size;

    Proxy operator[](const int);
    Matrix(const size_t, const size_t);
    ~Matrix();
  };
}

#endif
