//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_DISJOINTSET_H_
#define NONOGRAM_INCLUDES_DISJOINTSET_H_

#include <numeric>
#include <vector>

class DisjointSet {
 public:
  DisjointSet() = default;
  explicit DisjointSet(std::size_t size);

  void Resize(std::size_t size);

  std::size_t size() const;
  void Add(std::size_t father);
  std::size_t FindRoot(std::size_t v) const;
  void Unite(std::size_t u, std::size_t v);

 protected:
  std::size_t size_ = 0;
  mutable std::vector<std::size_t> fa_;
  std::vector<std::size_t> rank_;
};

#endif //NONOGRAM_INCLUDES_DISJOINTSET_H_
