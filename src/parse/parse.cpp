#include "Parse.h"
#include <cstddef>
#include <fstream>
void add_edge(std::size_t vert1, std::size_t vert2,
              std::vector<Vertex> &vertex) {
  if (!vertex[vert1].search_neiborhood(vert2)) {
    vertex[vert1].neibor_.insert(vert2);
    vertex[vert2].neibor_.insert(vert1);
  }
}
void read_file(std::string name, std::vector<Vertex> &vertex,
               std::list<Face> &face) {
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
      for (int j = 0; j < 3; ++j)
        add_edge(vert[j], vert[(j + 1) % 3], vertex);
    }
  }
  fin.close();
}

void write_file(std::string name, std::vector<Vertex> &vertex,
                std::list<Face> &face) {
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
