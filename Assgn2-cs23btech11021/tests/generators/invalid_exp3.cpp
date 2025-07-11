#include <bits/stdc++.h>
using namespace std;

int main() {
  srand(time(0));
  int k = 90;
  int n = k * k;
  for (int no = 1; no <= 32; no <<= 1) {
    std::string filename =
        "tests/inputs/exp3/invalids/inp" + to_string(no) + ".txt";
    ofstream input(filename);
    input << no << " " << n << " " << "20\n";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        input << (1 + rand() % n) << " ";
      }
      input << "\n";
    }
    input.close();
  }
}
