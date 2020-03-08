#include "Simplify.h"
#include "Core.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stack>
struct Q_result {
  Vec<double, 3> position_;
  double cost_;
};

Q_result Q_calculate(std::vector<Vertex> &vs, std::size_t index1,
                     std::size_t index2) {
  Q_Matrix Q = vs[index1].ma_ + vs[index2].ma_;
  Q_result out;
  try {
    out.position_ = Q.max_point();
    out.cost_ = Q.cal_norm(out.position_);
  } catch (std::domain_error) {
    // compare which is min and use it as position and use it to calculate
    // cost
#ifdef DEBUG
    std::cout << "No solution" << std::endl;
#endif
    double cost[3];
    cost[0] = Q.cal_norm(vs[index1]);
    cost[1] = Q.cal_norm(vs[index2]);
    cost[2] = Q.cal_norm(0.5 * (vs[index1] + vs[index2]));
    int index = 0;
    double min_cost = cost[index];
    for (int i = 1; i < 3; ++i)
      if (cost[i] < min_cost) {
        min_cost = cost[i];
        index = i;
      }
    switch (index) {
    case 0:
      out.position_ = vs[index1];
      break;
    case 1:
      out.position_ = vs[index2];
      break;
    case 2:
      out.position_ = 0.5 * (vs[index1] + vs[index2]);
      break;
    }
    out.cost_ = min_cost;
  }
  return out;
}

Simplify::Simplify(std::vector<Vertex> &vs, std::list<Face> &fs,
                   double threshold)
    : vertexs_(vs), faces_(fs), threshold2_(threshold * threshold),
      vertnum_(vs.size()) {
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
        if (Vec<double, 3>(vs[i] - vs[j]).norm2() < threshold2_) {
          heap_.push_back(Edge(i, j));
        } else {
          continue;
        }
      }
      Edge &new_pair = heap_.back();
      Q_result result = Q_calculate(vs, i, j);
      new_pair.cost_ = result.cost_;
      new_pair.position_ = result.position_;
    }
  }
  std::make_heap(heap_.begin(), heap_.end());
  for (std::size_t i = 0; i < heap_.size(); ++i) {
    vs[heap_[i].pair_.first].pair_location_.insert(i);
    vs[heap_[i].pair_.second].pair_location_.insert(i);
  }
}

void Simplify::swap_pairs(std::size_t index1, std::size_t index2) {
  bool iscontained[4] = {false, false, false, false};
  if (vertexs_[heap_[index1].pair_.first].pair_location_.erase(index1))
    iscontained[0] = true;
  if (vertexs_[heap_[index1].pair_.second].pair_location_.erase(index1))
    iscontained[1] = true;
  if (vertexs_[heap_[index2].pair_.first].pair_location_.erase(index2))
    iscontained[2] = true;
  if (vertexs_[heap_[index2].pair_.second].pair_location_.erase(index2))
    iscontained[3] = true;
  std::swap(heap_[index1], heap_[index2]);
  if (iscontained[2])
    vertexs_[heap_[index1].pair_.first].pair_location_.insert(index1);
  if (iscontained[3])
    vertexs_[heap_[index1].pair_.second].pair_location_.insert(index1);
  if (iscontained[0])
    vertexs_[heap_[index2].pair_.first].pair_location_.insert(index2);
  if (iscontained[1])
    vertexs_[heap_[index2].pair_.second].pair_location_.insert(index2);
}

void Simplify::downflow(std::size_t index) {
  if (heap_[index].isdeleted_) {
    swap_pairs(index, heap_.size() - 1);
    heap_.pop_back();
    downflow(index);
  }
  if ((index << 1) + 2 > heap_.size()) {
    heap_[index].ischanged_ = false;
    return;
  }
  std::size_t largest = index, left = (index << 1) + 1,
              right = (index << 1) + 2;
  if (heap_[left].ischanged_) {
    heap_[left].ischanged_ = false;
    downflow(left);
  }
  if (heap_[right].ischanged_) {
    heap_[right].ischanged_ = false;
    downflow(right);
  }
  if (heap_[largest] < heap_[left])
    largest = left;
  if (right != heap_.size() && heap_[largest] < heap_[right])
    largest = right;
  if (largest != index) {
    swap_pairs(largest, index);
    downflow(largest);
  }
}

void Simplify::remove() {
  downflow(0);
  std::size_t fin = heap_.front().pair_.first, sin = heap_.front().pair_.second;
  Vertex &fir = vertexs_[heap_.front().pair_.first],
         &sec = vertexs_[heap_.front().pair_.second];
  fir = heap_[0].position_;
  fir.ma_ = fir.ma_ + sec.ma_;
  sec.isdeleted_ = true;
  sec.index_ = fir.index_;
  fir.pair_location_.erase(0);
  sec.pair_location_.erase(0);
  for (std::size_t const &s : fir.pair_location_) {
    Q_result out =
        Q_calculate(vertexs_, heap_[s].pair_.first, heap_[s].pair_.second);
    heap_[s].cost_ = out.cost_;
    heap_[s].position_ = out.position_;
    heap_[s].ischanged_ = true;
  }
  std::stack<std::size_t> tmp;
  for (std::size_t const &s : sec.pair_location_) {
    if (heap_[s].pair_.first == sin)
      heap_[s].pair_.first = fin;
    if (heap_[s].pair_.second == sin)
      heap_[s].pair_.second = fin;
    for (std::size_t const &c : fir.pair_location_) {
      if (heap_[s] == heap_[c]) {
        tmp.push(s);
        break;
      }
    }
    if (tmp.empty() || tmp.top() != s) {
      Q_result out =
          Q_calculate(vertexs_, heap_[s].pair_.first, heap_[s].pair_.second);
      fir.pair_location_.insert(s);
      heap_[s].cost_ = out.cost_;
      heap_[s].position_ = out.position_;
      heap_[s].ischanged_ = true;
    }
  }
  while (!tmp.empty()) {
    heap_[tmp.top()].ischanged_ = true;
    heap_[tmp.top()].isdeleted_ = true;
    tmp.pop();
  }
  heap_[0].ischanged_ = true;
  heap_[0].isdeleted_ = true;
}

void Simplify::simplify(std::size_t aim) {
  while (vertnum_-- > aim) {
    remove();
#ifdef DEBUG
    std::cout << vertnum_ - aim << std::endl;
#endif
  }
}
