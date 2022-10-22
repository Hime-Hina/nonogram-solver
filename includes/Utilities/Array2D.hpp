//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_
#define NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Definitions.hpp"

template <typename T>
class Array2D {
 public:
  Array2D(size_t rows, size_t cols, const T &val = 0)
      : _rows(rows), _cols(cols), _arr(rows * cols, val) {}
  Array2D(const Array2D &arr)
      : _rows(arr._rows), _cols(arr._cols), _arr(arr._arr) {}
  Array2D(Array2D &&arr) noexcept
      : _rows(arr._rows), _cols(arr._cols), _arr(std::move(arr._arr)) {}
  Array2D(IList<IList<T>> list);

  Array2D &operator=(const Array2D &arr);
  Array2D &operator=(Array2D &&arr) noexcept;

  size_t rows() const { return _rows; }
  size_t cols() const { return _cols; }
  typename std::vector<T>::const_iterator operator()(size_t row) const {
    return _arr.cbegin() + row * _cols;
  }
  typename std::vector<T>::const_iterator cend(size_t row) const {
    return _arr.cbegin() + (row + 1) * _cols;
  }
  typename std::vector<T>::iterator operator()(size_t row) {
    return _arr.begin() + row * _cols;
  }
  typename std::vector<T>::iterator end(size_t row) {
    return _arr.begin() + (row + 1) * _cols;
  }
  typename std::vector<T>::const_reference operator()(size_t row, size_t col) const {
    return _arr[row * _cols + col];
  }
  typename std::vector<T>::reference operator()(size_t row, size_t col) {
    return _arr[row * _cols + col];
  }

 private:
  size_t _rows{}, _cols{};
  std::vector<T> _arr;
};

template <typename T>
Array2D<T>::Array2D(IList<IList<T>> list) : _rows(list.size()) {
  _cols = 0;
  for (auto &l : list) {
    _cols = std::max(_cols, l.size());
  }
  _arr.resize(_rows * _cols);
  for (auto lit = list.begin(), lie = list.end(), ait = _arr.begin();
       lit != lie; ++lit, ait += _cols) {
    std::copy(lit->begin(), lit->end(), ait);
  }
}

template <typename T>
Array2D<T> &Array2D<T>::operator=(const Array2D &arr) {
  _rows = arr._rows;
  _cols = arr._cols;
  _arr = arr._arr;
  return *this;
}

template <typename T>
Array2D<T> &Array2D<T>::operator=(Array2D &&arr) noexcept {
  _rows = arr._rows;
  _cols = arr._cols;
  _arr = std::move(arr._arr);
  return *this;
}

#endif //NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_
