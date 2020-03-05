#include "Vertex.h"
Vertex::Vertex(double x, double y, double z, std::size_t index)
    : Vec<double, 3>({x, y, z}), isdeleted_(false), index_(index), ma_(), neibor_(), pair_location_() {}

Vertex::Vertex(double pos[3], std::size_t index)
    : Vec<double, 3>({pos[0], pos[1], pos[2]}), isdeleted_(false),
      index_(index), ma_(), neibor_(), pair_location_() {}

bool Vertex::search_neiborhood(std::size_t index) {
  // at first, Vertex's rank is equal to Vertex's index
  if (neibor_.find(index) == neibor_.end())
    return false;
  return true;
}

std::ostream &operator<<(std::ostream &out, Vertex const &o) {
  out << 'v' << ' ' << o[0] << ' ' << o[1] << ' ' << o[2];
  return out;
}
