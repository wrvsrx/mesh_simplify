#pragma once
#include "Matrix.h"
#include "Vec.h"
class Q_Matrix : public Matrix<double, 4, 4> {
public:
  Q_Matrix();
  Q_Matrix(Matrix<double, 4, 4> const &);
  Vec<double, 3> max_point();
  double cal_norm(Vec<double, 3> const &);
  double cal_norm(Vec<double, 4> const &);
  void add(Vec<double, 4> const &paramater);
};
