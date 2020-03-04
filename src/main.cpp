#include "Matrix.h"
#include "core.h"
#include "parse.h"
#include <iostream>
#include <string>
using namespace std;
int main() {
  Matrix<double, 3, 3> m1({{1, 1, 0}, {1, 2, 0}, {0, 0, 1}});
  Matrix<double, 3, 3> m2(m1.inverse());
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j)
      cout << m2[i][j] << ' ';
    cout << endl;
  }
  return 0;
}
