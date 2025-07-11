#include <bits/stdc++.h>
using namespace std;

int main() {
  int k = 90;
  int n = k * k;
  for (int no = 1; no <= 32; no <<= 1) {
    int rootn = (int)(sqrt(n) + 0.001);
    int start = 0;
    string filename = "tests/inputs/exp3/valids/inp" + to_string(no) + ".txt";
    ofstream input(filename);
    input << no << " " << n << " 20\n";
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
