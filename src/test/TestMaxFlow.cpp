#include "Graph/MaxFlow.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> row = {4, 3, 2, 3, 1};
  std::vector<int> col = {1, 2, 3, 4, 3};

  size_t tot = row.size() + col.size() + 2;
  Graph::AdjacencyList g(tot);

  for (int i = 0; i < row.size(); ++i) {
    g.AddEdge(Graph::Edge(0, i + 1).Capacity(row[i]));
  }
  for (int i = 1; i <= row.size(); ++i) {
    for (int j = row.size() + 1; j < tot - 1; ++j) {
      g.AddEdge(Graph::Edge(i, j).Capacity(1));
    }
  }
  for (int i = 0; i < col.size(); ++i) {
    g.AddEdge(Graph::Edge(row.size() + i + 1, tot - 1).Capacity(col[i]));
  }

  Graph::Maxflow mf;
  mf(g, 0, tot - 1);

  auto &&flows = mf.flows();
  std::vector<std::vector<int>> f(tot, std::vector<int>(tot, 0));

  for (auto &&p : flows) {
    for (auto &&pp : p.second) {
      f[p.first][pp.first] = pp.second;
    }
  }
  for (auto &i : f) {
    for (int j : i) {
      std::cout << std::setw(3) << j;
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  return 0;
}
