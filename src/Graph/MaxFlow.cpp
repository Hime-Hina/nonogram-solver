//
// Created by HimeHina on 2022/10/21.
//

#include "Graph/MaxFlow.hpp"

namespace Graph {

const Maxflow &Maxflow::operator()(const AdjacencyList &g, int src, int sink) {
  AdjacencyList rg(CreateResidualGraph_(g));

  src_ = src;
  sink_ = sink;

  max_flow_ = 0;
  while (BFS_(rg)) {
    start_.assign(rg.total_v(), 0);
    max_flow_ += DFS_(rg, src_, inf);
  }

  int i = 0;
  flows_.clear();
  ITERATE_VERTICES_TMP(u, rg.total_v()) {
    ITERATE_EDGES(e, rg[u]) { flows_[e.from][e.to] = e.flow; }
  }

  return *this;
}

AdjacencyList Maxflow::CreateResidualGraph_(const AdjacencyList &g) {
  AdjacencyList rg(g);

  ITERATE_VERTICES_TMP(u, g.total_v()) {
    for (int i = 0; i < g[u].size(); ++i) {
      auto &&e = rg[u][i];
      e.rev = rg[e.to].size();
      rg.AddEdge(Edge(e.to, e.from).Weight(e.weight).Rev(i));
    }
  }
  return rg;
}

bool Maxflow::BFS_(const AdjacencyList &g) {
  int u;
  iQueue q;

  level_.assign(g.total_v(), -1);

  u = src_;
  q.push(u);
  level_[u] = 0;
  while (!q.empty()) {
    u = q.front();
    q.pop();

    ITERATE_EDGES(e, g[u]) {
      int v = e.to;
      if (level_[v] == -1 && e.capacity > e.flow) {
        q.push(v);
        level_[v] = level_[u] + 1;
      }
    }
  }

  return level_[sink_] != -1;
}

int Maxflow::DFS_(AdjacencyList &g, int u, int augment) {
  if (u == sink_ || augment == 0) return augment;

  int flows = 0;
  for (int &i = start_[u]; i < g[u].size(); ++i) {
    auto &&e = g[u][i];
    int v = e.to;
    if (level_[v] == level_[u] + 1 && e.capacity > e.flow) {
      int flow = DFS_(g, v, std::min(augment, e.capacity - e.flow));
      if (flow > 0) {
        e.flow += flow;
        g[e.to][e.rev].flow -= flow;
        flows += flow;
        augment -= flow;
        if (augment == 0) break;
      }
    }
  }
  return flows;
}
} // Graph