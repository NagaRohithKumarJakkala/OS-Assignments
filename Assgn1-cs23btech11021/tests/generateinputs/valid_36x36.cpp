#include <bits/stdc++.h>
using namespace std;

int main() {
  for (int k = 2; k <= 32; k <<= 1) {
    int n = 36;
    int rootn = 6;
    int start = 0;
    string filename = "../inputs/36x36_valid/inp_t" + to_string(k) + ".txt";
    ofstream input(filename);
    input << k << " " << n << "\n";
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
