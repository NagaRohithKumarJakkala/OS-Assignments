#include <bits/stdc++.h>
using namespace std;

int main() {
  srand(time(0));
  for (int k = 2; k <= 32; k <<= 1) {
    int n = 36;
    std::string filename =
        "../inputs/36x36_invalid/inp_t" + to_string(k) + ".txt";
    ofstream input(filename);
    input << k << " " << n << "\n";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        input << (1 + rand() % n) << " ";
      }
      input << "\n";
    }
    input.close();
  }
}
