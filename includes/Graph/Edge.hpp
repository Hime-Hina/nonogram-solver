//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_EDGE_H_
#define NONOGRAM_INCLUDES_EDGE_H_

#include "Definitions.hpp"

namespace Graph {

struct Edge {
  const std::size_t from, to;
  int weight, capacity;
  int rev, flow;

  struct HashFn {
    template <typename T>
    static void hash_combine(std::size_t &seed, const T &v) {
      std::hash<T> h;
      seed ^= h(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    std::size_t operator()(const Edge &e) const {
      std::size_t res = 0;
      hash_combine(res, e.from);
      hash_combine(res, e.to);
      return res;
    }
  };

  Edge(std::size_t u, std::size_t v, int w = 1, int c = 0, int r = -1, int f = 0)
      : from(u), to(v), weight(w), capacity(c), rev(r), flow(f) {}

  Edge &Weight(int w) {
    weight = w;
    return *this;
  }
  Edge &Capacity(int c) {
    capacity = c;
    return *this;
  }
  Edge &Rev(int r) {
    rev = r;
    return *this;
  }
  Edge &Flow(int f) {
    flow = f;
    return *this;
  }

  bool operator==(const Edge &e) const {
    return from == e.from && to == e.to && weight == e.weight &&
        capacity == e.capacity;
  }

  bool operator<(const Edge &e) const {
#define COMPARE(a, op, b) \
  if ((a) != (b)) return (a)op(b)
    COMPARE(from, <, e.from);
    COMPARE(to, <, e.to);
    COMPARE(weight, <, e.weight);
    COMPARE(capacity, <, e.capacity);
    return false;
  }

  friend std::ostream &operator<<(std::ostream &out, const Edge &e) {
    out << '(' << e.from << ',' << e.to << ',' << e.weight << ',' << e.capacity
        << ')';
    return out;
  }
};

} // Graph

#endif //NONOGRAM_INCLUDES_EDGE_H_
