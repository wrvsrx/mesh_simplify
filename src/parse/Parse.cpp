#include "Parse.h"
#include <cstddef>
#include <fstream>
#include <sstream>

void add_edge(std::size_t vert1, std::size_t vert2,
              std::vector<Vertex> &vertex) {
  if (!vertex[vert1].search_neiborhood(vert2)) {
    vertex[vert1].neibor_.insert(vert2);
    vertex[vert2].neibor_.insert(vert1);
  }
}

void read_file(std::string name, std::vector<Vertex> &vertexs,
               std::list<Face> &faces) {
  std::ifstream fin;
  fin.open(name);
  std::string line, chara;
  std::size_t v_num = 0;
  std::string tmp;
  double position[3];
  std::size_t vert[3];
  while (getline(fin, line)) {
    if (line[0] == 'v') {
      if (line[1] != 'n') {
        std::istringstream sin(line);
        sin >> chara >> position[0] >> position[1] >> position[2];
        vertexs.push_back(Vertex(position, v_num++));
      }
    } else if (line[0] == 'f') {
      std::istringstream sin(line);
      sin >> chara;
      for (int i = 0; i < 3; ++i) {
        std::string stmp;
        sin >> stmp;
        std::size_t tmp = 0;
        for (int j = 0; stmp[j] != '/' && stmp[j] != 0; ++j)
          tmp = tmp * 10 + (stmp[j] - '0');
        vert[i] = --tmp;
      }
      faces.push_back(Face(vert));
    }
  }
  for (Face &f : faces) {
    for (int i = 0; i < 3; ++i) {
      add_edge(f.vertex_[i], f.vertex_[(i + 1) % 3], vertexs);
      vertexs[f.vertex_[i]].face_in_neibor_.insert(&f);
    }
  }
  fin.close();
}

void write_file(std::string name, std::vector<Vertex> &vertex,
                std::list<Face> &face) {
  std::ofstream fout;
  fout.open(name);
  std::size_t ind = 0;
  for (Vertex &v : vertex) {
    if (!v.isdeleted_) {
      fout << v << std::endl;
      v.index_ = ind++;
    }
  }
  for (Face const &f : face) {
    if (!f.isdeleted_)
      fout << 'f' << ' ' << vertex[f.vertex_[0]].index_ + 1 << ' '
           << vertex[f.vertex_[1]].index_ + 1 << ' '
           << vertex[f.vertex_[2]].index_ + 1 << std::endl;
  }
  fout.close();
}
