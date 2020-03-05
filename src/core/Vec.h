#pragma once
#include "Matrix.h"
#include <initializer_list>
template <class T, int dim> class Vec : public Matrix<T, dim, 1> {
public:
  Vec();
  Vec(std::initializer_list<T>);
  Vec(Vec<T, dim> const &);
  Vec(Matrix<T, dim, 1> const &);
  T &operator[](int index);
  T const &operator[](int index) const;
  Matrix<T, 1, dim> inverse() const;
  T norm() const;
};

template <class T, int dim1, int dim2>
Vec<double, 3> cross(Vec<T, dim1> const &, Vec<T, dim2> const &);

template <class T, int dim> Vec<T, dim>::Vec() : Matrix<T, dim, 1>() {}

template <class T, int dim> Vec<T, dim>::Vec(std::initializer_list<T> init) {
  int i = 0;
  for (T const &l : init) {
    this->data_[i++][0] = l;
    if (i >= dim)
      break;
  }
}

template <class T, int dim>
Vec<T, dim>::Vec(Vec<T, dim> const &r) : Matrix<T, dim, 1>(r) {}

template <class T, int dim>
Vec<T, dim>::Vec(Matrix<T, dim, 1> const &r) : Matrix<T, dim, 1>(r) {}

template <class T, int dim> T &Vec<T, dim>::operator[](int index) {
  return this->data_[index][0];
}

template <class T, int dim> T const &Vec<T, dim>::operator[](int index) const {
  return this->data_[index][0];
}

template <class T, int dim> Matrix<T, 1, dim> Vec<T, dim>::inverse() const {
  Matrix<T, 1, dim> out;
  for (int i = 0; i < dim; ++i)
    out[0][i] = this->data_[i][0];
  return out;
}

template <class T, int dim> T Vec<T, dim>::norm() const {
  return (this->inverse() * (*this))[0][0];
}

template <class T, int dim>
T inner(Vec<T, dim> const &l, Vec<T, dim> const &r) {
  return l.inverse() * r;
}

template <class T, int dim1, int dim2>
Vec<double, 3> cross(Vec<T, dim1> const &l, Vec<T, dim2> const &r) {
  Vec<double, 3> out({0, 0, 0});
  for (int i = 0; i < 3; ++i)
    out[i] = l[(i + 1) % 3] * r[(i + 2) % 3] - l[(i + 2) % 3] * r[(i + 1) % 3];
  return out;
}
