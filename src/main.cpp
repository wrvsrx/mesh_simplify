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
  char opt[] = "i:o:p:t:vh";
  std::string inputname, outputname;
  double proportion, threshold = 0;
  bool isverbose = false, isinput = false, isoutput = false,
       isproportion = false;
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
    case 'v':
      isverbose = true;
      break;
    case 'h':
      std::cout << "./mesh [options]\n"
                << "Options:\n"
                << "\t-i [input file name] \t\t\t\t Set input file name (must)\n"
                << "\t-o [output file name] \t\t\t\t Set output file name (must)\n"
                << "\t-p [proportion] \t\t\t\t Set proportion (must)\n\n"
                << "\t-t [threshold] \t\t\t\t Set threhold (optional) (it will make building quite slow)\n\n"
                << "\t-v \t\t\t\t Show verbose information (optional)\n"
                << "\t-h \t\t\t\t Show help information (optional)\n"
                << std::endl;
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
  Simplify sim(vs, fs, threshold, isverbose);
  sim.simplify(fs.size() * proportion);
  write_file(outputname, vs, fs);
  return 0;
}
