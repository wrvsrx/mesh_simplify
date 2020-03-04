#pragma once
#include "Edge.h"
#include "Q_matrix.h"
#include "Vec.h"
#include <initializer_list>
#include <iostream>
#include <utility>
#include <vector>
class Vertex : public Vec<double, 3> {

public:
  Vertex(double x, double y, double z, std::size_t index);
  Vertex(double pos[3], std::size_t index);
  Edge *search_neiborhood(std::size_t index);
  Edge *search_neiborhood(Edge const &pa);
  friend std::ostream &operator<<(std::ostream &out, Vertex const &o);
  bool isdeleted_;
  std::size_t index_;
  Q_Matrix ma_;
  std::vector<Edge *> pair_set_;
};

