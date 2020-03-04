#include "parse.h"
#include <cstddef>
#include <fstream>
void add_edge(std::size_t vert1, std::size_t vert2, std::vector<Vertex> &vertex,
              std::list<Edge> &edge) {
  Edge tmp(vert1, vert2);
  if (!vertex[vert1].search_neiborhood(tmp)) {
    edge.push_back(tmp);
    vertex[vert1].pair_set_.push_back(&(edge.back()));
    vertex[vert2].pair_set_.push_back(&(edge.back()));
  }
}
void read_file(std::string name, std::vector<Vertex> &vertex,
               std::list<Edge> &edge, std::list<Face> &face) {
  std::ifstream fin;
  fin.open(name);
  char *chara = new char[128];
  std::string tmp;
  double position[3];
  std::size_t vert[3];
  for (std::size_t i = 0; !fin.eof(); ++i) {
    fin >> chara;
    if (fin.eof())
      break;
    if (*chara == '#') {
      getline(fin, tmp);
      --i;
    } else if (*chara == 'v') {
      fin >> position[0] >> position[1] >> position[2];
      vertex.push_back(Vertex(position, i));
    } else if (*chara == 'f') {
      fin >> vert[0] >> vert[1] >> vert[2];
      for (int i = 0; i < 3; ++i)
        --vert[i];
      face.push_back(Face(vert));
      add_edge(vert[0], vert[1], vertex, edge);
      add_edge(vert[0], vert[2], vertex, edge);
    }
  }
  fin.close();
}

void write_file(std::string name, std::vector<Vertex> &vertex,
                std::list<Edge> &edge, std::list<Face> &face) {
  std::ofstream fout;
  fout.open(name);
  for (Vertex const &v : vertex) {
    if (!v.isdeleted_) {
      fout << v << std::endl;
    }
  }
  for (Face const &f : face) {
    std::size_t ver[3];
    for (int i = 0; i < 3; ++i) {
      ver[i] = vertex[f.vertex_[i]].index_;
    }
    if (ver[0] != ver[1] && ver[1] != ver[2] && ver[2] != ver[0]) {
      fout << 'f' << ' ' << ver[0] + 1 << ' ' << ver[1] + 1 << ' ' << ver[2] + 1
           << std::endl;
    }
  }
  fout.close();
}
