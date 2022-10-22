//
// Created by HimeHina on 2022/10/23.
//
#include <random>
#include <chrono>

#include "Tester.hpp"
#include "Utilities/Array2D.hpp"

bool TestRowsAndCols() {
  Array2D<int> arr(329, 932);
  return arr.rows() == 329 && arr.cols() == 932;
}

bool TestInitContainerConstructor_initializer_list() {
  Array2D<int> arr({
                       {0, 1, 2, 3, 4},
                       {5, 6, 7, 8, 9},
                       {10, 11, 12, 13, 14},
                       {15, 16, 17, 18, 19},
                   });

  bool res = arr.rows() == 4 && arr.cols() == 5;
  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      res = res && (i * arr.cols() + j == arr(i, j));
    }
  }
  return res;
}
bool TestInitContainerConstructor_vector() {
  int rows = 512, cols = 512;
  std::vector<std::vector<int>> a(rows, std::vector<int>(cols, 0));

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      a[i][j] = dist(engine);
    }
  }

  Array2D<int> arr(a);

  bool res = arr.rows() == rows && arr.cols() == cols;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      res = res && (a[i][j] = arr(i, +j));
    }
  }
  return res;
}

bool TestFlattenToArray() {
  int rows = 512, cols = 512;
  std::vector<std::vector<int>> a(rows, std::vector<int>(cols, 0));

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      a[i][j] = dist(engine);
    }
  }

  std::size_t max_cols = 0;
  std::vector<int> arr = Array2D<int>::FlattenToArray_(a, max_cols);

  bool res = arr.size() == rows * cols;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      res = res && (a[i][j] = arr[i * cols + j]);
    }
  }
  return res;
}

int main() {
  Tester tester("Array2D Tests");

  tester
      .AddTest(TestRowsAndCols)
      .AddTest(TestInitContainerConstructor_initializer_list)
      .AddTest(TestInitContainerConstructor_vector)
      .AddTest(TestFlattenToArray)
      .Run();

  return 0;
}