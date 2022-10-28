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
  template <
      class InitArray,
      std::enable_if_t<
          std::conjunction_v<
              std::is_array<InitArray>,
              std::is_same<
                  std::integral_constant<std::size_t, std::rank_v<InitArray>>,
                  std::integral_constant<std::size_t, 2>
              >
          >,
          bool
      > = true
  >
  explicit Array2D(const InitArray &array)
      : _rows(std::extent_v<InitArray, 0>), _cols(std::extent_v<InitArray, 1>),
        _arr(std::extent_v<InitArray, 0> * std::extent_v<InitArray, 1>) {
    for (std::size_t i = 0; i < _rows; ++i) {
      for (std::size_t j = 0; j < _cols; ++j) {
        _arr[i * _cols + j] = array[i][j];
      }
    }
  }
  template <
      template <class...> class InitContainer,
      std::enable_if_t<
          is_iterable_v<InitContainer<InitContainer<T>>>, bool
      > = true
  >
  explicit Array2D(const InitContainer<InitContainer<T>> &container)
      : _rows(container.size()), _cols(0) {
    std::size_t rows = container.size();
    _cols = 0;
    for (auto &l : container) {
      _cols = std::max(_cols, l.size());
    }

    _arr.resize(rows * _cols);
    auto ait = _arr.begin();
    for (auto it = container.begin(), ie = container.end(); it != ie; ++it, ait += _cols) {
      std::copy(it->begin(), it->end(), ait);
    }
  }

  void resize(std::size_t rows, std::size_t cols) {
    _rows = rows;
    _cols = cols;
    _arr.resize(rows * cols);
  }

  void assign(std::size_t rows, std::size_t cols, const T &val) {
    _rows = rows;
    _cols = cols;
    _arr.assign(rows * cols, val);
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

 private:
  std::size_t _rows, _cols;
  std::vector<T> _arr;
};

#endif //NONOGRAM_INCLUDES_UTILITIES_ARRAY2D_HPP_
