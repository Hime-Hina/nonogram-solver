//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_DISJOINTSET_H_
#define NONOGRAM_INCLUDES_DISJOINTSET_H_

#include <cstddef>
#include <numeric>
#include <vector>

class DisjointSet {
 public:
  DisjointSet() = default;
  explicit DisjointSet(size_t size);

  void Resize(size_t size);

  size_t size() const;
  void Add(size_t father);
  size_t FindRoot(size_t v) const;
  void Unite(size_t u, size_t v);

 protected:
  size_t size_ = 0;
  mutable std::vector<size_t> fa_;
  std::vector<size_t> rank_;
};

#endif //NONOGRAM_INCLUDES_DISJOINTSET_H_
