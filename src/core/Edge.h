#pragma once
#include "Vec.h"
#include <cstddef>
#include <utility>

class Edge {
public:
  Edge(std::size_t rank1, std::size_t rank2);
  double cost_;
  Vec<double, 3> position_;

protected:
  std::pair<std::size_t, std::size_t> pair_;

  friend bool operator==(Edge const &, Edge const &);
  friend bool operator<(Edge const &, Edge const &);
};
