#include <bits/stdc++.h>
using namespace std;

int main() {
  srand(time(0));
  for (int k = 20; k <= 100; k += 10) {
    int n = k * k;
    std::string filename =
        "tests/inputs/exp1/invalids/inp" + to_string(n) + ".txt";
    ofstream input(filename);
    input << 8 << " " << n << " " << "20\n";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        input << (1 + rand() % n) << " ";
      }
      input << "\n";
    }
    input.close();
  }
}
