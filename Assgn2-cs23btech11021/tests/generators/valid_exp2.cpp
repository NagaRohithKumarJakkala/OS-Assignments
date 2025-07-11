#include <bits/stdc++.h>
using namespace std;

int main() {
  int k = 90;
  int n = k * k;
  int rootn = (int)(sqrt(n) + 0.001);
  for (int taskInc = 10; taskInc <= 50; taskInc += 5) {
    int start = 0;
    string filename =
        "tests/inputs/exp2/valids/inp" + to_string(taskInc) + ".txt";
    ofstream input(filename);
    input << 8 << " " << n << " " << taskInc << "\n";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        input << ((start + j) % n) + 1 << " ";
      }
      input << "\n";
      start += rootn;
      if (start >= n) {
        start = ((start) % n) + 1;
      }
    }
    input.close();
  }
  return 0;
}
