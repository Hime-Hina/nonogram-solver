//
// Created by HimeHina on 2022/10/21.
//

#include "Graph/TopologicalSort.hpp"

const Graph::TopologicalSort &Graph::TopologicalSort::operator()(const Graph::AdjacencyList &g) {
  int u;
  iArray d_in(g.degrees_in());

  ITERATE_VERTICES(u, g.total_v()) {
    if (d_in[u] == 0) break;
  }
  if (u >= g.total_v()) return *this;

  Flags visited(g.total_v(), false);
  std::queue<int> q;

  q.push(u);
  visited[u] = true;
  while (!q.empty()) {
    u = q.front();
    q.pop();

    res.emplace_back(u);
    ITERATE_EDGES(e, g[u]) {
      int v = e.to;
      if (!visited[v] && --d_in[v] == 0) {
        q.push(v);
        visited[v] = true;
      }
    }
  }
  if (res.size() != g.total_v()) {
    res.clear();
  }

  return *this;
}
