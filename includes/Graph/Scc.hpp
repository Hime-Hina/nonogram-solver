//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_GRAPH_SCC_HPP_
#define NONOGRAM_INCLUDES_GRAPH_SCC_HPP_

#include "AdjacencyList.hpp"

namespace Graph {

class SCC {
 public:
  const SCC &operator()(const AdjacencyList &g);

  size_t scc_counter() const { return scc_counter_; }
  int belongs(int u) const { return belongs_[u]; }
  size_t size_of(int component) const { return size_of_[component]; }
  bool IsConnected(int u, int v) { return djs_.FindRoot(u) == djs_.FindRoot(v); }

 protected:
  size_t scc_counter_;
  iArray dfn_, low_, belongs_, size_of_;
  DisjointSet djs_;

  void Tarjan_(const AdjacencyList &g, int u, int &time_stamp, iStack &stk, Flags &is_in_stk);
};

} // Graph

#endif //NONOGRAM_INCLUDES_GRAPH_SCC_HPP_
