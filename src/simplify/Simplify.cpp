#include "Simplify.h"
#include "Core.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stack>
struct Q_result {
  Vec<double, 3> position_;
  double cost_;
};

Q_result Q_calculate(std::vector<Vertex> &vs, std::size_t index1,
                     std::size_t index2);

bool isreverse(std::vector<Vertex> &vs, std::size_t index,
               Vec<double, 3> new_position);

Simplify::Simplify(std::vector<Vertex> &vs, std::list<Face> &fs,
                   double threshold, bool isverbose, bool isjudgereverse)
    : vertexs_(vs), faces_(fs), threshold2_(threshold * threshold),
      facenum_(fs.size()), isverbose_(isverbose),
      isjudgereverse_(isjudgereverse) {
  using std::size_t;
  if (isverbose_) {
    std::cout << "build start" << std::endl;
  }
  size_t v_number = vertexs_.size();
  for (Face &f : fs) {
    f.set_parameter(vs);
    for (int i = 0; i < 3; ++i) {
      vs[f.vertex_[i]].ma_.add(f.paramater_);
    }
  }
  if (isverbose_) {
    std::cout << "calculating paramater finish" << std::endl;
  }
  for (size_t i = 0; i < v_number; ++i) {
    if (threshold2_ == 0)
      for (size_t const &j : vs[i].neibor_) {
        if (j < i)
          continue;
        heap_.push_back(Edge(i, j));
        Edge &new_pair = heap_.back();
        Q_result result = Q_calculate(vs, i, j);
        new_pair.cost_ = result.cost_;
        if (isjudgereverse_ && (isreverse(vs, i, result.position_) ||
                                isreverse(vs, j, result.position_)))
          new_pair.cost_ += 100;
        new_pair.position_ = result.position_;
      }
    else
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
  if (isverbose_) {
    std::cout << "build finish" << std::endl;
    std::cout << "total " << vertexs_.size() << " vertexs" << std::endl;
    std::cout << "total " << heap_.size() << " pairs" << std::endl;
    std::cout << "total " << faces_.size() << " faces" << std::endl;
  }
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
  // fir.ma_ = Q_Matrix();
  fir.ma_ = fir.ma_ + sec.ma_;
  sec.isdeleted_ = true;
  sec.index_ = fir.index_;
  for (Face *const &f : sec.face_in_neibor_) {
    if (!f->isdeleted_) {
      for (int i = 0; i < 3; ++i) {
        if (f->vertex_[i] == fin) {
          f->isdeleted_ = true;
          facenum_--;
          break;
        } else if (f->vertex_[i] == sin) {
          f->vertex_[i] = fin;
        }
      }
      if (!f->isdeleted_) {
        fir.face_in_neibor_.insert(f);
      }
    }
  }
  sec.face_in_neibor_.clear();
  // for (Face *const &f : fir.face_in_neibor_) {
    // if (!f->isdeleted_) {
      // f->set_parameter(vertexs_);
      // fir.ma_.add(f->paramater_);
    // }
  // }
  fir.pair_location_.erase(0);
  sec.pair_location_.erase(0);
  for (std::size_t const &s : sec.pair_location_) {
    if (heap_[s].pair_.first == sin)
      heap_[s].pair_.first = fin;
    if (heap_[s].pair_.second == sin)
      heap_[s].pair_.second = fin;
    bool isduplicate = false;
    for (std::size_t const &c : fir.pair_location_) {
      if (heap_[s] == heap_[c]) {
        heap_[s].ischanged_ = true;
        heap_[s].isdeleted_ = true;
        isduplicate = true;
        break;
      }
    }
    if (!isduplicate)
      fir.pair_location_.insert(s);
  }
  for (std::size_t const &s : fir.pair_location_) {
    Q_result out =
        Q_calculate(vertexs_, heap_[s].pair_.first, heap_[s].pair_.second);
    // if (isverbose_ && out.cost_ < heap_[s].cost_)
      // std::cout << "cost reduced ";
    heap_[s].cost_ = out.cost_;
    if (isjudgereverse_ &&
        (isreverse(vertexs_, heap_[s].pair_.first, out.position_) ||
         isreverse(vertexs_, heap_[s].pair_.second, out.position_)))
      heap_[s].cost_ += 100;
    heap_[s].position_ = out.position_;
    heap_[s].ischanged_ = true;
  }
  heap_[0].ischanged_ = true;
  heap_[0].isdeleted_ = true;
}

void Simplify::simplify(std::size_t aim) {
  while (facenum_ > aim)
    remove();
  if (isverbose_) {
    std::cout << facenum_ << " faces remained" << std::endl;
  }
}

Q_result Q_calculate(std::vector<Vertex> &vs, std::size_t index1,
                     std::size_t index2) {
  Q_Matrix Q = vs[index1].ma_ + vs[index2].ma_;
  Q_result out;
  try {
    out.position_ = Q.max_point();
    out.cost_ = Q.cal_norm(out.position_);
  } catch (std::domain_error) {
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

bool isreverse(std::vector<Vertex> &vs, std::size_t index,
               Vec<double, 3> new_position) {
  for (Face *const &f : vs[index].face_in_neibor_) {
    if (!f->isdeleted_) {
      Vec<double, 3> old_direct;
      Vec<double, 3> new_direct;
      for (int i = 0; i < 3; ++i) {
        old_direct[i] = f->paramater_[i];
      }
      Vec<double, 3> new_vertex[3];
      for (int i = 0; i < 3; ++i) {
        if (f->vertex_[i] == index) {
          new_vertex[i] = new_position;
        } else
          new_vertex[i] = vs[f->vertex_[i]];
      }
      new_direct = cross(Vec<double, 3>(new_vertex[1] - new_vertex[0]),
                         Vec<double, 3>(new_vertex[2] - new_vertex[1]));
      if (inner(old_direct, new_direct) < 0)
        return true;
    }
  }
  return false;
}
