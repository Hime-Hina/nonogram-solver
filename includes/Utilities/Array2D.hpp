//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_
#define NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_

#include "Definitions.hpp"

template <typename T>
class Array2D {
 public:
  Array2D(std::size_t rows, std::size_t cols, const T &val = 0)
      : _rows(rows), _cols(cols), _arr(rows * cols, val) {}
  Array2D(const Array2D &arr)
      : _rows(arr._rows), _cols(arr._cols), _arr(arr._arr) {}
  Array2D(Array2D &&arr) noexcept
      : _rows(arr._rows), _cols(arr._cols), _arr(std::move(arr._arr)) {}
  template <template <class, class...> class InitContainer = std::initializer_list>
  explicit Array2D(InitContainer<InitContainer<T>> container);

  Array2D &operator=(const Array2D &arr);
  Array2D &operator=(Array2D &&arr) noexcept;

  std::size_t rows() const { return _rows; }
  std::size_t cols() const { return _cols; }
  std::vector<T> arr() const { return _arr; }
  typename std::vector<T>::const_iterator operator()(std::size_t row) const {
    return _arr.cbegin() + row * _cols;
  }
  typename std::vector<T>::const_iterator cend(std::size_t row) const {
    return _arr.cbegin() + (row + 1) * _cols;
  }
  typename std::vector<T>::iterator operator()(std::size_t row) {
    return _arr.begin() + row * _cols;
  }
  typename std::vector<T>::iterator end(std::size_t row) {
    return _arr.begin() + (row + 1) * _cols;
  }
  typename std::vector<T>::const_reference operator()(std::size_t row, std::size_t col) const {
    return _arr[row * _cols + col];
  }
  typename std::vector<T>::reference operator()(std::size_t row, std::size_t col) {
    return _arr[row * _cols + col];
  }

  template <class Iterable>
  static auto FlattenToArray_(const Iterable &iterable, std::size_t &max_cols)
  -> decltype(iterable.size(), iterable.begin(), iterable.end(), std::vector<T>{}) {
    std::vector<T> arr;

    std::size_t rows = iterable.size();
    max_cols = 0;
    for (auto &l : iterable) {
      max_cols = std::max(max_cols, l.size());
    }

    arr.resize(rows * max_cols);
    auto ait = arr.begin();
    for (auto it = iterable.begin(), ie = iterable.end(); it != ie; ++it, ait += max_cols) {
      std::copy(it->begin(), it->end(), ait);
    }
    return arr;
  }

 private:
  std::size_t _rows, _cols;
  std::vector<T> _arr;
};

template <typename T>
template <template <class, class...> class InitContainer>
Array2D<T>::Array2D(InitContainer<InitContainer<T>> container)
    : _rows(container.size()), _cols(0) {
  _arr = FlattenToArray_(container, _cols);
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
