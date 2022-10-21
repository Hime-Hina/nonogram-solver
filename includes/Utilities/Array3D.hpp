//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_UTILITIES_ARRAY3D_HPP_
#define NONOGRAM_INCLUDES_UTILITIES_ARRAY3D_HPP_

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
class Array3D {
  template <typename U>
  using IList = std::initializer_list<U>;

 public:
  Array3D(size_t rows, size_t cols, size_t aisles)
      : _rows(rows),
        _cols(cols),
        _aisles(aisles),
        _offset(cols * aisles),
        _arr(rows * cols * aisles) {}
  Array3D(size_t rows, size_t cols, size_t aisles, const T &val)
      : _rows(rows),
        _cols(cols),
        _aisles(aisles),
        _offset(cols * aisles),
        _arr(rows * cols * aisles, val) {}
  Array3D(const Array3D &arr)
      : _rows(arr._rows),
        _cols(arr._cols),
        _aisles(arr._aisles),
        _offset(arr._cols * arr._aisles),
        _arr(arr._arr) {}
  Array3D(Array3D &&arr) noexcept
      : _rows(arr._rows),
        _cols(arr._cols),
        _aisles(arr._aisles),
        _offset(arr._cols * arr._aisles),
        _arr(std::move(arr._arr)) {}
  Array3D(IList<IList<IList<T>>> list);

  Array3D &operator=(const Array3D &arr);
  Array3D &operator=(Array3D &&arr) noexcept;

  size_t rows() const { return _rows; }
  size_t cols() const { return _cols; }
  size_t aisle() const { return _aisles; }
  T operator()(size_t row, size_t col, size_t aisle) const {
    return _arr[row * _offset + col * _aisles + aisle];
  }

 private:
  size_t _rows, _cols, _aisles;
  size_t _offset;
  std::vector<T> _arr;
};

template <typename T>
Array3D<T>::Array3D(Array3D::IList<Array3D::IList<Array3D::IList<T>>> list) : _rows(list.size()) {
  _cols = 0;
  _aisles = 0;
  for (auto &l : list) {
    _cols = std::max(_cols, l.size());
    for (auto &ll : l) {
      _aisles = std::max(_aisles, ll.size());
    }
  }
  _offset = _cols * _aisles;

  _arr.resize(_rows * _cols * _aisles);
  for (auto lit = list.begin(), lie = list.end(), ait = _arr.begin();
       lit != lie; ++lit, ait += _offset) {
    for (auto llit = lit->begin(), llie = lit->end(), aiit = ait;
         llit != llie; ++llit, aiit += _aisles) {
      std::copy(llit->begin(), llit->end(), aiit);
    }
  }
}

template <typename T>
Array3D<T> &Array3D<T>::operator=(const Array3D &arr) {
  _rows = arr._rows;
  _cols = arr._cols;
  _aisles = arr._aisles;
  _offset = arr._offset;
  _arr = arr._arr;
  return *this;
}

template <typename T>
Array3D<T> &Array3D<T>::operator=(Array3D &&arr) noexcept {
  _rows = arr._rows;
  _cols = arr._cols;
  _aisles = arr._aisles;
  _offset = arr._offset;
  _arr = std::move(arr._arr);
  arr._arr.clear();
  return *this;
}

#endif //NONOGRAM_INCLUDES_UTILITIES_ARRAY3D_HPP_
