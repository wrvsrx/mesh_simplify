#pragma once
#include "Core.h"
#include <list>
#include <utility>
#include <vector>
class Simplify {
public:
  Simplify(std::vector<Vertex> &, std::list<Face> &, double threshold = 0);
  void simplify(std::size_t num);

protected:
  std::vector<Vertex> &vertexs_;
  std::list<Face> &faces_;
  double threshold2_;
  std::size_t vertnum_;
  std::vector<Edge> heap_;
  void swap_pairs(std::size_t index1, std::size_t index2);
  void downflow(std::size_t index);
  void overflow(std::size_t index);
  void update_cost(std::size_t index);
  void remove();
};
