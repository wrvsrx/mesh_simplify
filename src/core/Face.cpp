#include "Face.h"
#include <cstddef>
#include <vector>
#include <cmath>

Face::Face(std::size_t vert[3]){
  for (int i = 0; i < 3; ++i) {
    vertex_[i] = vert[i];
  }
}

void Face::set_parameter(std::vector<Vertex> const & pa) {
  Vertex const * v[3];
  for (int i = 0; i < 3; ++i)
    v[i] = &(pa[vertex_[i]]);
  int isnegative = 1;
  for (int i = 0; i < 4; ++i) {
    paramater_[i] = 0;
    for (int j = 0; j < 3; ++j) {
      double tmp = 1;
      for (int k = 0; k < 3; ++k)
        tmp *= (*v)[k][((j + k) % 3 + i + 1) % 4];
      paramater_[i] += tmp;
    }
    for (int j = 0; j < 3; ++j) {
      double tmp = 1;
      for (int k = 0; k < 3; ++k) 
        tmp *= (*v)[k][((j - k + 3) % 3 + i + 1) % 4];
      paramater_[i] -= tmp;
    }
    paramater_[i] *= isnegative;
    isnegative *= -1;
  }
  double norm = sqrt(paramater_.norm());
  paramater_ = (1 / norm) * paramater_;
}

