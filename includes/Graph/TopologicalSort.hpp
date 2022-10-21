//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_GRAPH_TOPOLOGICALSORT_HPP_
#define NONOGRAM_INCLUDES_GRAPH_TOPOLOGICALSORT_HPP_

#include "AdjacencyList.hpp"

namespace Graph {
class TopologicalSort {
 public:
  const TopologicalSort &operator()(const AdjacencyList &g);

  iArray res;
};
}

#endif //NONOGRAM_INCLUDES_GRAPH_TOPOLOGICALSORT_HPP_
