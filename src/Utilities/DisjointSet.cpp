//
// Created by HimeHina on 2022/10/21.
//

#include "Utilities/DisjointSet.h"

DisjointSet::DisjointSet(std::size_t size) : size_(size), fa_(size), rank_(size, 1) {
  std::iota(fa_.begin(), fa_.end(), 0);
}

void DisjointSet::Resize(std::size_t size) {
  size_ = size;
  fa_.resize(size_);
  rank_.assign(size_, 1);
  std::iota(fa_.begin(), fa_.end(), 0);
}

std::size_t DisjointSet::size() const { return size_; }

void DisjointSet::Add(std::size_t father) { fa_.emplace_back(father); }

std::size_t DisjointSet::FindRoot(std::size_t v) const {
  if (fa_[v] == v) return v;
  return fa_[v] = FindRoot(fa_[v]);
}

void DisjointSet::Unite(std::size_t u, std::size_t v) {
  u = FindRoot(u), v = FindRoot(v);
  if (u == v) return;

  if (rank_[u] < rank_[v]) std::swap(u, v);
  fa_[v] = u;
  rank_[u] += rank_[v];
}
