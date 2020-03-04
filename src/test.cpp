#include <iostream>
#include <vector>
using namespace std;
int main() {
  vector<int> a;
  a.push_back(10);
  a.push_back(20);
  for (int & i: a){
    i = 0;
  }
  for (int & i: a){
    cout << i << endl; 
  }
}

