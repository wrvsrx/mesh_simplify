#include "Core.h"
#include "Parse.h"
#include "Simplify.h"
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;
#ifdef DEBUG
std::size_t sum(vector<Vertex> const &vs) {
  std::size_t out = 0;
  for (Vertex const &v : vs)
    if (!v.isdeleted_)
      ++out;
  return out;
}
#endif
int main() {
  std::string name("../resources/bunny.obj");
  std::vector<Vertex> vs;
  std::list<Face> fs;
  read_file(name, vs, fs);
  Simplify sim(vs, fs);
  sim.simplify(100);
  write_file("../output/test.obj", vs, fs);
  return 0;
}
