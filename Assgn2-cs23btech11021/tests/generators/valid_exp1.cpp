#include <bits/stdc++.h>
using namespace std;

int main() {
  for (int k = 20; k <= 100; k += 10) {
    int n = k * k;
    int rootn = k;
    int start = 0;
    string filename = "tests/inputs/exp1/valids/inp" + to_string(n) + ".txt";
    ofstream input(filename);
    input << 8 << " " << n << " 20\n";
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
