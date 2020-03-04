#pragma once
#include "Matrix.h"
#include "Vec.h"
class Q_Matrix : public Matrix<double, 4, 4> {
public:
  Q_Matrix();
  Q_Matrix(Matrix<double, 4, 4> const &);
  Matrix<double, 4, 4> inverse_derivate();
  void add(Vec<double, 4> const &paramater);
};
