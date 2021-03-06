#pragma once
#include <exception>
#include <initializer_list>
#include <stdexcept>
template <class T, int dim1, int dim2> class Matrix {
public:
  Matrix();
  Matrix(T[dim1][dim2]);
  Matrix(std::initializer_list<std::initializer_list<T>>);
  Matrix(Matrix<T, dim1, dim2> const &);
  T *operator[](int index);
  T const *operator[](int index) const;
  Matrix<T, dim1, dim2> operator-() const;
  T determinate(T const &zero_limit = T(0));
  Matrix<T, dim1, dim2> inverse(T const &zero_limit = T(0));
  void swap_row(int r1, int r2);
  void swap_column(int c1, int c2);

protected:
  T data_[dim1][dim2];
};

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator+(Matrix<T, dim1, dim2> const &l,
                                Matrix<T, dim1, dim2> const &r);

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator*(T const &l, Matrix<T, dim2, dim2> const &r);

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
Matrix<T, dim1, dim2>::Matrix(Matrix<T, dim1, dim2> const &r) {
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      data_[i][j] = r[i][j];
}

template <class T, int dim1, int dim2>
T *Matrix<T, dim1, dim2>::operator[](int index) {
  return data_[index];
}

template <class T, int dim1, int dim2>
T const *Matrix<T, dim1, dim2>::operator[](int index) const {
  return data_[index];
}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> Matrix<T, dim1, dim2>::operator-() const {
  Matrix<T, dim1, dim2> out;
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      out[i][j] = - data_[i][j];
  return out;
}

template <class T, int dim1, int dim2> T Matrix<T, dim1, dim2>::determinate(T const &zero_limit) {
  if (dim1 != dim2) {
    std::domain_error unconsistent_dim("unconsistent dim");
    throw(unconsistent_dim);
  }
  Matrix<T, dim1, dim2> tmp = *this;
  int n = dim1;
  bool isnegative = false;
  T const zero(0), one(1);
  for (int i = 0; i < n; ++i) {
    if (tmp[i][i] < zero_limit && -zero_limit < tmp[i][i]) {
      for (int j = i + 1; j < n; ++j) {
        if (tmp[j][i] > zero_limit || tmp[j][i] < -zero_limit) {
          tmp.swap_row(i, j);
          isnegative = !isnegative;
          break;
        }
      }
      return zero;
    }
    for (int j = i + 1; j < n; ++j) {
      T alpha = tmp[j][i] / tmp[i][i];
      for (int k = i + 1; k < n; ++k)
        tmp[j][k] -= tmp[i][k] * alpha;
    }
  }
  T out(one);
  for (int i = 0; i < n; ++i)
    out *= tmp[i][i];
  return isnegative ? -out : out;
}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> Matrix<T, dim1, dim2>::inverse(T const & zero_limit) {
  if (dim1 != dim2) {
    std::domain_error unconsistent_dim("unconsistent dim");
    throw(unconsistent_dim);
  }
  Matrix<T, dim1, dim2> tmp(*this), out;
  int n = dim1;
  T const zero(0), one(1);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      out[i][j] = zero;
  for (int i = 0; i < n; ++i)
    out[i][i] = one;
  for (int i = 0; i < n; ++i) {
    if (tmp[i][i] < zero_limit && -zero_limit < tmp[i][i]) {
      for (int j = i + 1; j < n; ++j) {
        if (tmp[j][i] > zero_limit || tmp[j][i] < -zero_limit) {
          tmp.swap_row(i, j);
          out.swap_row(i, j);
          break;
        }
        std::domain_error unconsistent_dim("not inversable");
        throw(unconsistent_dim);
      }
    }
    for (int j = 0; j < n; ++j) {
      if (j != i) {
        T alpha = tmp[j][i] / tmp[i][i];
        for (int k = 0; k < n; ++k) {
          out[j][k] -= out[i][k] * alpha;
          tmp[j][k] -= tmp[i][k] * alpha;
        }
      }
    }
  }
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) {
      out[i][j] *= one / tmp[i][i];
    }
  return out;
}

template <class T, int dim1, int dim2>
void Matrix<T, dim1, dim2>::swap_row(int r1, int r2) {
  for (int i = 0; i < dim2; ++i)
    std::swap(data_[r1][i], data_[r2][i]);
}

template <class T, int dim1, int dim2>
void Matrix<T, dim1, dim2>::swap_column(int c1, int c2) {
  for (int i = 0; i < dim1; ++i)
    std::swap(data_[i][c1], data_[i][c2]);
}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator+(Matrix<T, dim1, dim2> const &l,
                                Matrix<T, dim1, dim2> const &r) {
  Matrix<T, dim1, dim2> out;
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      out[i][j] = l[i][j] + r[i][j];
  return out;
}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator-(Matrix<T, dim1, dim2> const &l,
                                Matrix<T, dim1, dim2> const &r) {
  Matrix<T, dim1, dim2> out;
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      out[i][j] = l[i][j] - r[i][j];
  return out;
}

template <class T, int dim1, int dim2>
Matrix<T, dim1, dim2> operator*(T const &l, Matrix<T, dim1, dim2> const &r) {
  Matrix<T, dim1, dim2> out;
  for (int i = 0; i < dim1; ++i)
    for (int j = 0; j < dim2; ++j)
      out[i][j] = l * r[i][j];
  return out;
}

template <class T, int dim11, int dim12, int dim22>
Matrix<T, dim11, dim22> operator*(Matrix<T, dim11, dim12> const &l,
                                  Matrix<T, dim12, dim22> const &r) {
  Matrix<T, dim11, dim22> out;
  for (int i = 0; i < dim11; ++i)
    for (int j = 0; j < dim22; ++j) {
      out[i][j] = T(0);
      for (int k = 0; k < dim12; ++k)
        out[i][j] += l[i][k] * r[k][j];
    }
  return out;
}
