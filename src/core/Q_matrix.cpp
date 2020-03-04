#include "Q_matrix.h"
Q_Matrix::Q_Matrix() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      data_[i][j] = 0;
}

Q_Matrix::Q_Matrix(Matrix<double, 4, 4> const &r) : Matrix<double, 4, 4>(r) {}

// Q_Matrix Q_Matrix::inverse_derivate() {}

void Q_Matrix::add(Vec<double, 4> const &paramater) {
  *this = *this + paramater * paramater.inverse();
}
