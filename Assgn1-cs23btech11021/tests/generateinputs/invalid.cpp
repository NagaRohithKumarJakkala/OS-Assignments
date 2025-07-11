#include <bits/stdc++.h>
using namespace std;

int main() {
  srand(time(0));
  for (int k = 2; k <= 25; k++) {
    int n = k * k;
    std::string filename =
        "../inputs/invalids/inp" + to_string(n) + "x" + to_string(n) + ".txt";
    ofstream input(filename);
    input << 8 << " " << n << "\n";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        input << (1 + rand() % n) << " ";
      }
      input << "\n";
    }
    input.close();
  }
}
