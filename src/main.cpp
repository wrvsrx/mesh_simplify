#include "Core.h"
#include "Parse.h"
#include "Simplify.h"
#include <getopt.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;
int main(int argc, char *argv[]) {
  int o;
  char opt[] = "i:o:p:t:rvh";
  std::string inputname, outputname;
  double proportion, threshold = 0;
  bool isverbose = false, isinput = false, isoutput = false,
  isproportion = false, isjudgereverse = false;
  std::vector<Vertex> vs;
  std::list<Face> fs;
  while ((o = getopt(argc, argv, opt)) != -1) {
  switch (o) {
  case 'i':
  inputname = optarg;
  isinput = true;
  break;
  case 'o':
  outputname = optarg;
  isoutput = true;
  break;
  case 'p':
  proportion = std::stod(optarg);
  isproportion = true;
  break;
  case 't':
  threshold = std::stod(optarg);
  break;
  case 'r':
  isjudgereverse = true;
  break;
  case 'v':
  isverbose = true;
  break;
  case 'h':
  std::cout
  << "./mesh [options]\n"
  << "Options:\n"
  << "\t-i [input file name]        Set input file name (must)\n"
  << "\t-o [output file name]       Set output file name (must)\n"
  << "\t-p [proportion]             Set proportion (must)\n"
  << "\t-t [threshold]              Set threhold (optional) (it will make "
  "building quite slow)\n"
  << "\t-r                          Set \"judging reverse\" to true (optional) (it will make "
  << "\t-v                          Show verbose information (optional)\n"
  << "\t-h                          Show help information (optional)\n"
  << "Examples:\n"
  << "./mesh -i ../resources/armadillo.obj -o ../output/armadillo.obj -p 0.001 -v"
  << std::endl;
  return 0;
  break;
  }
  }
  if (!isinput) {
  std::cout << "No input file name." << std::endl;
  return 0;
  }
  if (!isoutput) {
  std::cout << "No output file name." << std::endl;
  return 0;
  }
  if (!isproportion) {
  std::cout << "No proportion." << std::endl;
  return 0;
  }
  read_file(inputname, vs, fs);
  Simplify sim(vs, fs, threshold, isverbose, isjudgereverse);
  sim.simplify(fs.size() * proportion);
  write_file(outputname, vs, fs);
  // std::vector<Vertex> vs = {Vertex(0, 0, 0, 0), Vertex(0, 0, 1, 0),
                            // Vertex(1, 0, 0, 0), Vertex(0, 1, 0, 0)};
  // std::size_t vers[3] = {1, 2, 3};
  // Face a(vers);
  // a.set_parameter(vs);
  // vs[1].face_in_neibor_.insert(&a);
  // std::cout << isinvert(vs, 1, Vec<double, 3>({1, 1, -1}));
  return 0;
}
