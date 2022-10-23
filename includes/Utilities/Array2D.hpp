//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_
#define NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_

#include <map>
#include "Definitions.hpp"
#include "UtilityTemplates.hpp"

template <typename T>
class Array2D {
 public:
  Array2D(std::size_t rows, std::size_t cols, const T &val = 0)
      : _rows(rows), _cols(cols), _arr(rows * cols, val) {}
  Array2D(const Array2D &arr)
      : _rows(arr._rows), _cols(arr._cols), _arr(arr._arr) {}
  Array2D(Array2D &&arr) noexcept
      : _rows(arr._rows), _cols(arr._cols), _arr(std::move(arr._arr)) {}

  template <
      typename InitContainer,
      std::enable_if_t<
          std::conjunction_v<
              is_container<std::remove_reference_t<InitContainer>>,
              std::is_same<T, typename std::remove_reference_t<InitContainer>::value_type>
          >, bool
      > = true
  >
  Array2D(std::size_t rows, std::size_t cols, InitContainer &&container)
      : _rows(rows), _cols(cols), _arr(container.begin(), container.end()) {
    if (_arr.size() != _rows * _cols)
      throw std::invalid_argument("The scale does not match the size of container!");
  }

  template <template <class, class...> class InitContainer = std::initializer_list>
  explicit Array2D(InitContainer<InitContainer<T>> container)
      : _rows(container.size()), _cols(0) {
    _arr = FlattenToArray_(container, _cols);
  }

  Array2D<T> &operator=(const Array2D &arr) {
    _rows = arr._rows;
    _cols = arr._cols;
    _arr = arr._arr;
    return *this;
  }

  Array2D<T> &operator=(Array2D &&arr) noexcept {
    _rows = arr._rows;
    _cols = arr._cols;
    _arr = std::move(arr._arr);
    return *this;
  }

  std::size_t rows() const { return _rows; }
  std::size_t cols() const { return _cols; }

  typename std::vector<T>::const_iterator cbegin(std::size_t row = 0) const {
    return _arr.cbegin() + row * _cols;
  }
  typename std::vector<T>::const_iterator cend(std::size_t row = 0) const {
    return _arr.cbegin() + (row + 1) * _cols;
  }
  typename std::vector<T>::iterator begin(std::size_t row = 0) {
    return _arr.begin() + row * _cols;
  }
  typename std::vector<T>::iterator end(std::size_t row = 0) {
    return _arr.begin() + (row + 1) * _cols;
  }
  typename std::vector<T>::iterator at(std::size_t index = 0) {
    return _arr.begin() + index;
  }
  typename std::vector<T>::const_reference operator()(std::size_t row, std::size_t col) const {
    return _arr[row * _cols + col];
  }
  typename std::vector<T>::reference operator()(std::size_t row, std::size_t col) {
    return _arr[row * _cols + col];
  }
  typename std::vector<T>::const_reference operator()(std::size_t index = 0) const {
    return _arr[index];
  }
  typename std::vector<T>::reference operator()(std::size_t index = 0) {
    return _arr[index];
  }

  template <
      class Iterable,
      std::enable_if_t<
          is_iterable_v<Iterable>, bool
      > = true
  >
  static std::vector<T> FlattenToArray_(const Iterable &iterable, size_t &max_cols) {
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

#endif //NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_
