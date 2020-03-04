#include "Vertex.h"
Vertex::Vertex(double x, double y, double z, std::size_t index)
    : Vec<double, 3>({x, y, z}), isdeleted_(false), index_(index), ma_(),
      pair_set_() {}

Vertex::Vertex(double pos[3], std::size_t index)
    : Vec<double, 3>({pos[0], pos[1], pos[2]}), isdeleted_(false),
      index_(index), ma_(), pair_set_() {}

Edge *Vertex::search_neiborhood(std::size_t index) {
  // at first, Vertex's rank is equal to Vertex's index
  Edge tmp(index_, index);
  return search_neiborhood(tmp);
}

Edge *Vertex::search_neiborhood(Edge const &pa) {
  for (Edge *const &p : pair_set_) {
    if (*p == pa) {
      return p;
    }
  }
  return 0;
}

std::ostream &operator<<(std::ostream &out, Vertex const &o) {
  out << 'v' << ' ' << o[0] << ' ' << o[1] << ' ' << o[2];
  return out;
}

