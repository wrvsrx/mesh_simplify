#include "Face.h"
#include "Vec.h"
#include <cmath>
#include <cstddef>
#include <vector>

Face::Face(std::size_t vert[3]) {
  for (int i = 0; i < 3; ++i) {
    vertex_[i] = vert[i];
  }
}

void Face::set_parameter(std::vector<Vertex> const &pa) {
  Vec<double, 4> v[3];
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j)
      v[i][j] = pa[vertex_[i]][j];
    v[i][3] = 1;
  }
  int isnegative = 1;
  for (int i = 0; i < 4; ++i) {
    paramater_[i] = 0;
    for (int j = 0; j < 3; ++j) {
      double tmp = 1;
      for (int k = 0; k < 3; ++k)
        tmp *= v[k][((j + k) % 3 + i + 1) % 4];
      paramater_[i] += tmp;
    }
    for (int j = 0; j < 3; ++j) {
      double tmp = 1;
      for (int k = 0; k < 3; ++k)
        tmp *= v[k][((j - k + 3) % 3 + i + 1) % 4];
      paramater_[i] -= tmp;
    }
    paramater_[i] *= isnegative;
    isnegative *= -1;
  }
  double fake_norm = 0;
  for (int i = 0; i < 3; ++i)
    fake_norm += paramater_[i] * paramater_[i];
  fake_norm = sqrt(fake_norm);
  paramater_ = (1 / fake_norm) * paramater_;
}
