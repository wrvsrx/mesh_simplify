#include "Q_matrix.h"
#include <stdexcept>
Q_Matrix::Q_Matrix() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      data_[i][j] = 0;
}

Q_Matrix::Q_Matrix(Matrix<double, 4, 4> const &r) : Matrix<double, 4, 4>(r) {}

Vec<double, 3> Q_Matrix::max_point(double zero_limit) {
  Matrix<double, 3, 3> submatrix;
  Vec<double, 3> vec;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j)
      submatrix[i][j] = data_[i][j];
    vec[i] = data_[3][i];
  }
  return - (submatrix.inverse(zero_limit) * vec);
}

double Q_Matrix::cal_norm(Vec<double, 3> const &input) {
  Vec<double, 4> tmp;
  for (int i = 0; i < 3; ++i)
    tmp[i] = input[i];
  tmp[3] = 1;
  return cal_norm(tmp);
}

double Q_Matrix::cal_norm(Vec<double, 4> const &input) {
  return (input.transpose() * (*this) * input)[0][0];
}

void Q_Matrix::add(Vec<double, 4> const &paramater) {
  *this = *this + paramater * paramater.transpose();
}
