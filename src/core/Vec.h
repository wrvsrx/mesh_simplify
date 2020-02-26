#pragma once
#include "Matrix.h"
#include <initializer_list>
template <class T, int dim> class Vec : public Matrix<T, dim, 1> {
public:
  Vec(std::initializer_list<T>);
  T &operator[](int index);
  Matrix<T, 1, dim> inverse() const;
  T norm() const;
};

template <class T, int dim> Vec<T, dim>::Vec(std::initializer_list<T> init) {
  int i = 0;
  for (T const &l : init) {
    this->data_[i++][0] = l;
    if (i >= dim)
      break;
  }
}
template <class T, int dim> T &Vec<T, dim>::operator[](int index) {
  return this->data_[index][0];
}

template <class T, int dim> Matrix<T, 1, dim> Vec<T, dim>::inverse() const {
  Matrix<T, 1, dim> out;
  for (int i = 0; i < dim; ++i)
    out[0][i] = this->data_[i][0];
  return out;
}

template <class T, int dim> T Vec<T, dim>::norm() const {
  return (this->inverse() * (*this)).data_[0][0];
}

template <class T, int dim>
T inner(Vec<T, dim> const &l, Vec<T, dim> const &r) {
  return l.inverse() * r;
}
