#include "Edge.h"
Edge::Edge(std::size_t rank1, std::size_t rank2)
    : pair_(rank1, rank2), cost_(0), position_({0, 0, 0}) {}
bool operator==(Edge const &l, Edge const &r) {
  return (l.pair_.first == r.pair_.first && l.pair_.second == r.pair_.second) ||
         (l.pair_.first == r.pair_.second && l.pair_.second == r.pair_.first);
}
bool operator<(Edge const &l, Edge const &r) {
  return l.cost_ > r.cost_; // less cost has a high priority
}
