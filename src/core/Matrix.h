#pragma once
#include <initializer_list>
#include <iostream> // debug
template <class T, int dim1, int dim2> class Matrix {
public:
  Matrix();
  Matrix(T[dim1][dim2]);
  Matrix(std::initializer_list<std::initializer_list<T>>);
  T data_[dim1][dim2];
  T *operator[](int index);
};

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator+(Matrix<T, dim1, dim2> const &l,
                                Matrix<T, dim1, dim2> const &r);

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator-(Matrix<T, dim1, dim2> const &l,
                                Matrix<T, dim1, dim2> const &r);

template <class T, int dim11, int dim12, int dim22>
Matrix<T, dim11, dim22> operator*(Matrix<T, dim11, dim12> const &l,
                                  Matrix<T, dim12, dim22> const &r);

template <class T, int dim1, int dim2> Matrix<T, dim1, dim2>::Matrix() {}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2>::Matrix(T data[dim1][dim2]) {
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      data_[i][j] = data[i][j];
}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2>::Matrix(
    std::initializer_list<std::initializer_list<T>> data) {
  int i = 0, j = 0;
  for (std::initializer_list<T> const &data_ite : data) {
    for (T const &d : data_ite) {
      data_[i][j++] = d;
      if (j >= dim2) {
        j = 0;
        i++;
        break;
      }
    }
    if (i >= dim1)
      break;
  }
}
template <class T, int dim1, int dim2>
T *Matrix<T, dim1, dim2>::operator[](int index) {
  return data_[index];
}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator+(Matrix<T, dim1, dim2> const &l,
                                Matrix<T, dim1, dim2> const &r) {
  Matrix<T, dim1, dim2> out;
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      out[i][j] = l.data_[i][j] + r.data_[i][j];
  return out;
}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator-(Matrix<T, dim1, dim2> const &l,
                                Matrix<T, dim1, dim2> const &r) {
  Matrix<T, dim1, dim2> out;
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      out[i][j] = l.data_[i][j] - r.data_[i][j];
  return out;
}

template <class T, int dim11, int dim12, int dim22>
Matrix<T, dim11, dim22> operator*(Matrix<T, dim11, dim12> const &l,
                                  Matrix<T, dim12, dim22> const &r) {
  Matrix<T, dim11, dim22> out;
  for (int i = 0; i < dim11; ++i)
    for (int j = 0; j < dim22; ++j) {
      out[i][j] = l.data_[i][0] * r.data_[0][j];
      for (int k = 1; k < dim12; ++k)
        out[i][j] += l.data_[i][k] * r.data_[k][j];
    }
  return out;
}
