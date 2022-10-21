//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_GRAPH_ADJACENCYLIST_HPP_
#define NONOGRAM_INCLUDES_GRAPH_ADJACENCYLIST_HPP_

#include "Definitions.hpp"
#include "Utilities/DisjointSet.h"
#include "Edge.h"

namespace Graph {

class AdjacencyList {
 public:
  explicit AdjacencyList(size_t total_vertices)
      : total_vertices_(total_vertices),
        d_in_(total_vertices),
        d_out_(total_vertices),
        g_(total_vertices, EdgeSet()) {}
  AdjacencyList(const AdjacencyList &) = default;
  AdjacencyList(AdjacencyList &&) noexcept = default;

  size_t total_v() const { return total_vertices_; }
  int degree_in(int u) const { return d_in_[u]; }
  const iArray &degrees_in() const { return d_in_; }
  int degree_out(int u) const { return d_out_[u]; }
  const iArray &degrees_out() const { return d_out_; }

  AdjacencyList &AddEdge(const Edge &e);
  AdjacencyList &AddEdge(Edge &&e);
  AdjacencyList &AddEdge(int u, int v, int weight = 1, int capacity = 0, int rev = -1);

  void Print() const;

  EdgeSet &operator[](size_t u) { return g_[u]; }
  const EdgeSet &operator[](size_t u) const { return g_[u]; }

 protected:
  size_t total_vertices_;
  iArray d_in_, d_out_;
  AdjList g_;
};

} // Graph

#endif //NONOGRAM_INCLUDES_GRAPH_ADJACENCYLIST_HPP_
