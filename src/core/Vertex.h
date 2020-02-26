#pragma once
#include "Vec.h"
#include <initializer_list>
#include <utility>
#include <vector>
class Vertex : public Vec<double, 4> {
  using Edge = std::pair<Vertex, Vertex>;
  using Face = std::pair<Edge, Edge>;

public:
  std::vector<Face> faces_;
  Vertex(double x, double y, double z);
};

Vertex::Vertex(double x, double y, double z) : Vec<double, 4>({x, y, z, 1}) {}
