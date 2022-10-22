//
// Created by HimeHina on 2022/10/21.
//

#include "Graph/AdjacencyList.hpp"

namespace Graph {
AdjacencyList &AdjacencyList::AddEdge(const Edge &e) {
  g_[e.from].emplace_back(e);
  ++d_out_[e.from], ++d_in_[e.to];
  return *this;
}

AdjacencyList &AdjacencyList::AddEdge(int u, int v, int weight, int capacity, int rev) {
  g_[u].emplace_back(Edge(u, v, weight, capacity, rev));
  ++d_out_[u], ++d_in_[v];
  return *this;
}

void AdjacencyList::Print() const {
  ITERATE_VERTICES_TMP(u, total_vertices_) {
    std::cout << u << '(' << d_in_[u] << ',' << d_out_[u] << ')' << ": ";
    ITERATE_EDGES(e, g_[u]) { std::cout << e << ' '; }
    std::cout << '\n';
  }
}
} // Graph