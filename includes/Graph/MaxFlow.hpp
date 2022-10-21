//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_GRAPH_MAXFLOW_HPP_
#define NONOGRAM_INCLUDES_GRAPH_MAXFLOW_HPP_

#include "AdjacencyList.hpp"

namespace Graph {

class Maxflow {
 public:
  const Maxflow &operator()(const AdjacencyList &g, int src, int sink);

  int max_flow() const { return max_flow_; }
  const iiMapi &flows() const { return flows_; }

 protected:
  int src_, sink_, max_flow_;
  iArray level_, start_;
  iiMapi flows_;

  static AdjacencyList CreateResidualGraph_(const AdjacencyList &g);
  bool BFS_(const AdjacencyList &g);
  int DFS_(AdjacencyList &g, int u, int augment);
};

} // Graph

#endif //NONOGRAM_INCLUDES_GRAPH_MAXFLOW_HPP_
