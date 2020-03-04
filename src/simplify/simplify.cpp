#include "simplify.h"
#include "Face.h"
#include <iostream>
Simplify::Simplify(std::vector<Vertex> &vs, std::list<Edge> &es,
                   std::list<Face> &fs, double threshold)
    : vertexs_(vs), pairs_(es), faces_(fs),
      threshold_(threshold) {
  using std::size_t;
  size_t v_number = vertexs_.size(), f_number = pairs_.size();
  for (Face &f : fs) {
    f.set_parameter(vs);
    for (int i = 0; i < 3; ++i) {
      vs[f.vertex_[i]].ma_.add(f.paramater_);
    }
  }
  for (size_t i = 0; i < v_number; ++i) {
    for (size_t j = i + 1; j < v_number; ++j) {
    }
  }
}
