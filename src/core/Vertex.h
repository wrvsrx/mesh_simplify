#pragma once
#include "Edge.h"
#include "Q_Matrix.h"
#include "Vec.h"
#include "Face.h"
#include <initializer_list>
#include <iostream>
#include <utility>
#include <vector>
#include <set>
class Vertex : public Vec<double, 3> {

public:
  Vertex(double x, double y, double z, std::size_t index);
  Vertex(double pos[3], std::size_t index);
  bool search_neiborhood(std::size_t index);
  friend std::ostream &operator<<(std::ostream &out, Vertex const &o);
  Vertex &operator=(Vec<double, 3> const &);
  bool isdeleted_;
  std::size_t index_;
  Q_Matrix ma_;
  std::set<std::size_t> neibor_;
  std::set<std::size_t> pair_location_;
  std::set<Face *> face_in_neibor_;
};

