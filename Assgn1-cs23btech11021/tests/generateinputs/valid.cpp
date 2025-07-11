#include <bits/stdc++.h>
using namespace std;

int main() {
  for (int k = 2; k <= 25; k++) {
    int n = k * k;
    int rootn = (int)(sqrt(n) + 0.001);
    int start = 0;
    string filename =
        "../inputs/valids/inp" + to_string(n) + "x" + to_string(n) + ".txt";
    ofstream input(filename);
    input << 8 << " " << n << "\n";
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
