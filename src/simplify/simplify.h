#pragma once
#include "Core.h"
#include <list>
#include <utility>
#include <vector>
class Simplify {
public:
  Simplify(std::vector<Vertex> &, std::list<Face> &,
           double threshold = 0);

protected:
  std::vector<Vertex> &vertexs_;
  std::list<Face> &faces_;
  std::vector<Edge> heap_;
  double threshold2_;
};
