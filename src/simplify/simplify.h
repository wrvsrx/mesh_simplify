#pragma once
#include "core.h"
#include <list>
#include <utility>
#include <vector>
class Simplify {
public:
  Simplify(std::vector<Vertex> &, std::list<Edge> &, std::list<Face> &,
           double threshold = 0);

protected:
  std::vector<Vertex> &vertexs_;
  std::list<Edge> &pairs_;
  std::list<Face> &faces_;
  std::vector<Edge *> heap_;
  bool (*cmp)(Edge *const &, Edge *const &);
  double threshold2_;
};
