//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_UTILITIES_ARRAY3D_HPP_
#define NONOGRAM_INCLUDES_UTILITIES_ARRAY3D_HPP_

#include "Utilities/UtilityTemplates.hpp"
#include "Definitions.hpp"

template <typename T>
class Array3D {
 public:
  Array3D(std::size_t rows, std::size_t cols, std::size_t aisles, const T &val = 0)
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
  template <
      typename InitContainer,
      std::enable_if_t<
          std::conjunction_v<
              is_container<std::remove_reference_t<InitContainer>>,
              std::is_same<T, typename std::remove_reference_t<InitContainer>::value_type>
          >, bool
      > = true
  >
  Array3D(std::size_t rows, std::size_t cols, std::size_t aisles, InitContainer &&container)
      : _rows(rows), _cols(cols), _aisles(aisles), _offset(cols * aisles),
        _arr(container.begin(), container.end()) {
    if (_arr.size() != _rows * _offset)
      throw std::invalid_argument("The scale does not match the size of container!");
  }
  template <
      class InitArray,
      std::enable_if_t<
          std::conjunction_v<
              std::is_array<InitArray>,
              std::is_same<
                  std::integral_constant<std::size_t, std::rank_v<InitArray>>,
                  std::integral_constant<std::size_t, 3>
              >
          >,
          bool
      > = true
  >
  explicit Array3D(const InitArray &array)
      : _rows(std::extent_v<InitArray, 0>), _cols(std::extent_v<InitArray, 1>),
        _aisles(std::extent_v<InitArray, 2>),
        _offset(std::extent_v<InitArray, 1> * std::extent_v<InitArray, 2>),
        _arr(std::extent_v<InitArray, 0> * std::extent_v<InitArray, 1> * std::extent_v<InitArray, 2>) {
    for (std::size_t i = 0; i < _rows; ++i) {
      for (std::size_t j = 0; j < _cols; ++j) {
        for (std::size_t k = 0; k < _aisles; ++k) {
          _arr[(i * _cols + j) * _aisles + k] = array[i][j][k];
        }
      }
    }
  }
  template <
      template <class...> class InitContainer,
      std::enable_if_t<
          is_iterable_v<InitContainer<InitContainer<InitContainer<T>>>>, bool
      > = true
  >
  explicit Array3D(InitContainer<InitContainer<InitContainer<T>>> container)
      : _rows(container.size()), _cols(0), _aisles(0) {
    for (auto &l : container) {
      _cols = std::max(_cols, l.size());
    }
    for (auto &l : container) {
      _cols = std::max(_cols, l.size());
      for (auto &ll : l) {
        _aisles = std::max(_aisles, ll.size());
      }
    }
    _offset = _cols * _aisles;

    _arr.resize(_rows * _offset);
    auto ait = _arr.begin();
    for (auto it = container.begin(), ie = container.end(); it != ie; ++it, ait += _offset) {
      auto aiit = ait;
      for (auto iit = it->begin(), iie = it->end(); iit != iie; ++iit, aiit += _aisles) {
        std::copy(iit->begin(), iit->end(), aiit);
      }
    }
  }

  Array3D<T> &operator=(const Array3D &arr) {
    _rows = arr._rows;
    _cols = arr._cols;
    _aisles = arr._aisles;
    _offset = arr._offset;
    _arr = arr._arr;
    return *this;
  }
  Array3D<T> &operator=(Array3D &&arr) noexcept {
    _rows = arr._rows;
    _cols = arr._cols;
    _aisles = arr._aisles;
    _offset = arr._offset;
    _arr = std::move(arr._arr);
    arr._arr.clear();
    return *this;
  }

  std::size_t rows() const { return _rows; }
  std::size_t cols() const { return _cols; }
  std::size_t aisles() const { return _aisles; }

  typename std::vector<T>::const_iterator cbegin(std::size_t row) const {
    return _arr.cbegin() + row * _offset;
  }
  typename std::vector<T>::const_iterator cend(std::size_t row) const {
    return _arr.cbegin() + (row + 1) * _offset;
  }

  typename std::vector<T>::iterator begin(std::size_t row) {
    return _arr.begin() + row * _offset;
  }
  typename std::vector<T>::iterator end(std::size_t row) {
    return _arr.begin() + (row + 1) * _offset;
  }

  typename std::vector<T>::const_iterator cbegin(std::size_t row, std::size_t col) const {
    return _arr.cbegin() + row * _offset + col * _aisles;
  }
  typename std::vector<T>::const_iterator cend(std::size_t row, std::size_t col) const {
    return _arr.cbegin() + (row * _cols + col + 1) * _aisles;
  }

  typename std::vector<T>::iterator begin(std::size_t row, std::size_t col) {
    return _arr.begin() + row * _offset + col * _aisles;
  }
  typename std::vector<T>::iterator end(std::size_t row, std::size_t col) {
    return _arr.begin() + (row * _cols + col + 1) * _aisles;
  }

  typename std::vector<T>::const_reference operator()(std::size_t row, std::size_t col, std::size_t aisle) const {
    return _arr[row * _offset + col * _aisles + aisle];
  }
  typename std::vector<T>::reference operator()(std::size_t row, std::size_t col, std::size_t aisle) {
    return _arr[row * _offset + col * _aisles + aisle];
  }
  typename std::vector<T>::const_reference operator()(std::size_t index) const {
    return _arr[index];
  }
  typename std::vector<T>::reference operator()(std::size_t index) {
    return _arr[index];
  }

 private:
  std::size_t _rows, _cols, _aisles;
  std::size_t _offset;
  std::vector<T> _arr;
};

#endif //NONOGRAM_INCLUDES_UTILITIES_ARRAY3D_HPP_
