#include "Matrix.h"
#include "core.h"
#include "parse.h"
#include <iostream>
#include <string>
using namespace std;
int main() {
  Matrix<double, 3, 3> m1({{0, 1, 2}, {1, 2, 3}, {2, 3, 4}});
  cout << m1.determinate() << endl;
  return 0;
}
