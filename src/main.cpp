#include "core.h"
#include "parse.h"
#include "simplify.h"
#include <iostream>
#include <iterator> // debug
#include <list>
#include <string>
#include <vector>
using namespace std;
int main() {
  std::string name("../resources/test.obj");
  std::vector<Vertex> vs;
  std::list<Face> fs;
  read_file(name, vs, fs);
  Simplify sim(vs, fs);
  return 0;
}
