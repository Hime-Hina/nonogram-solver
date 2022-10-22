#include <iostream>
#include "Solver.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  Solver nonogram(n, m);

  nonogram.Input();
  nonogram.Solve();
  nonogram.Print(false);

  return 0;
}
