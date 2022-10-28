#include <iostream>
#include "Solver.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  Solver nonogram(n, m);

  std::cin >> nonogram;
  nonogram.Solve();
  std::cout << nonogram;

  return 0;
}
