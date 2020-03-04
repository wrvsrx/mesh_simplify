#pragma once
#include "Vertex.h"
#include <cstddef>
#include <vector>

class Face {
public:
  Face(std::size_t vert[3]);
  void set_parameter(std::vector<Vertex> const &);
  std::size_t vertex_[3];
  Vec<double, 4> paramater_;
};
