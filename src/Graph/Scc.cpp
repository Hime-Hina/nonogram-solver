//
// Created by HimeHina on 2022/10/21.
//

#include "Graph/Scc.hpp"

namespace Graph {
const SCC &SCC::operator()(const AdjacencyList &g) {
  size_t tot = g.total_v();
  int time_stamp = 0;
  Flags is_in_stk(tot, false);
  iStack stk;

  scc_counter_ = 0;
  dfn_.assign(tot, 0);
  low_.assign(tot, 0);
  belongs_.assign(tot, 0);
  size_of_.assign(tot, 0);
  djs_.Resize(tot);

  ITERATE_VERTICES_TMP(u, tot) {
    if (dfn_[u] == 0) Tarjan_(g, u, time_stamp, stk, is_in_stk);
  }

  return *this;
}

void SCC::Tarjan_(const AdjacencyList &g, int u, int &time_stamp, iStack &stk, Flags &is_in_stk) {
  dfn_[u] = low_[u] = ++time_stamp;
  stk.push(u);
  is_in_stk[u] = true;

  ITERATE_EDGES(e, g[u]) {
    int v = e.to;
    if (dfn_[v] == 0) {
      Tarjan_(g, v, time_stamp, stk, is_in_stk);
      low_[u] = std::min(low_[u], low_[v]);
    } else if (is_in_stk[v]) {
      low_[u] = std::min(low_[u], dfn_[v]);
    }
  }

  if (dfn_[u] == low_[u]) {
    int top;
    do {
      top = stk.top();
      stk.pop();
      belongs_[top] = scc_counter_;
      ++size_of_[scc_counter_];
      is_in_stk[top] = false;
      djs_.Unite(top, u);
    } while (top != u);
    ++scc_counter_;
  }
}
} // Graph