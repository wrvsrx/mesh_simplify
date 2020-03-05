#include "Simplify.h"
#include "Core.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
Simplify::Simplify(std::vector<Vertex> &vs,
                   std::list<Face> &fs, double threshold)
    : vertexs_(vs), faces_(fs),
      threshold2_(threshold * threshold) {
  using std::size_t;
  size_t v_number = vertexs_.size();
  for (Face &f : fs) {
    f.set_parameter(vs);
    for (int i = 0; i < 3; ++i) {
      vs[f.vertex_[i]].ma_.add(f.paramater_);
    }
  }
  for (size_t i = 0; i < v_number; ++i) {
    for (size_t j = i + 1; j < v_number; ++j) {
      bool isneibor = vs[i].search_neiborhood(j);
      if (isneibor) {
        heap_.push_back(Edge(i, j));
      } else {
        if (Vec<double, 3>(vs[i] - vs[j]).norm() < threshold2_) {
          heap_.push_back(Edge(i, j));
        }
        else {
          continue;
        }
      }
      Edge & new_pair = heap_.back();
      Q_Matrix Q = vs[i].ma_ + vs[j].ma_;
      try {
        new_pair.position_ = Q.max_point();
        new_pair.cost_ = Q.cal_norm(new_pair.position_);
      } catch (std::domain_error) {
        // compare which is min and use it as position and use it to calculate
        // cost
        double cost[3];
        cost[0] = Q.cal_norm(vs[i]);
        cost[1] = Q.cal_norm(vs[j]);
        cost[2] = Q.cal_norm(0.5 * (vs[j] + vs[j]));
        int index = 0;
        double min_cost = cost[index];
        for (int i = 1; i < 3; ++i)
          if (cost[i] < min_cost) {
            min_cost = cost[i];
            index = i;
          }
        switch (index) {
        case 0:
          new_pair.position_ = vs[i];
          break;
        case 1:
          new_pair.position_ = vs[i];
          break;
        case 2:
          new_pair.position_ = 0.5 * (vs[i] + vs[j]);
          break;
        }
        new_pair.cost_ = min_cost;
      }
    }
  }
  std::make_heap(heap_.begin(), heap_.end());
  for (std::size_t i = 0; i < heap_.size(); ++i) {
    vs[heap_[i].pair_.first].pair_location_.insert(i);
    vs[heap_[i].pair_.second].pair_location_.insert(i);
  }
}
