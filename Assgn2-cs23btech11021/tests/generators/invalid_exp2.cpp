#include <bits/stdc++.h>
using namespace std;

int main() {
  srand(time(0));
  int k = 90;
  int n = k * k;
  for (int taskInc = 10; taskInc <= 50; taskInc += 5) {
    std::string filename =
        "tests/inputs/exp2/invalids/inp" + to_string(taskInc) + ".txt";
    ofstream input(filename);
    input << 8 << " " << n << " " << taskInc << "\n";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        input << (1 + rand() % n) << " ";
      }
      input << "\n";
    }
    input.close();
  }
}
