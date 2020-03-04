#include "simplify.h"
#include "core.h"
#include <cstddef>
#include <iostream>
#include <queue>
Simplify::Simplify(std::vector<Vertex> &vs, std::list<Edge> &es,
                   std::list<Face> &fs, double threshold)
    : vertexs_(vs), pairs_(es), faces_(fs), threshold2_(threshold * threshold) {
  using std::size_t;
  size_t v_number = vertexs_.size(), f_number = pairs_.size();
  for (Face &f : fs) {
    f.set_parameter(vs);
    for (int i = 0; i < 3; ++i) {
      vs[f.vertex_[i]].ma_.add(f.paramater_);
    }
  }
  for (size_t i = 0; i < v_number; ++i) {
    for (size_t j = i + 1; j < v_number; ++j) {
      Edge *p = vs[i].search_neiborhood(j);
      if (!p) {
        if (Vec<double, 3>(vs[i] - vs[j]).norm() < threshold2_) {
          es.push_back(Edge(i, j));
          vs[i].pair_set_.push_back(&(es.back()));
          vs[j].pair_set_.push_back(&(es.back()));
          p = &(es.back());
        }
      }
      Q_Matrix Q = vs[i].ma_ + vs[j].ma_;
      try {
        p->position_ = Q.max_point();
        p->cost_ = Q.cal_norm(p->position_);
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
          p->position_ = vs[i];
          break;
        case 1:
          p->position_ = vs[i];
          break;
        case 2:
          p->position_ = 0.5 * (vs[i] + vs[j]);
          break;
        }
        p->cost_ = min_cost;
      }
      
    }
  }
}
