//
// Created by HimeHina on 2022/10/23.
//
#include <random>
#include <chrono>
#include <set>
#include <map>

#include "Tester.hpp"
#include "Utilities/Array2D.hpp"

bool TestRowsAndCols() {
  int a = 100;
  Array2D<int> arr(329, 932, a);
  return arr.rows() == 329 && arr.cols() == 932;
}

bool TestInitContainerConstructor_initializer_list() {
  IList<IList<int>> il = {
      {0, 1, 2, 3, 4},
      {5, 6, 7, 8, 9},
      {10, 11, 12, 13, 14},
      {15, 16, 17, 18, 19},
  };
  int aa[4][5] = {
      {0, 1, 2, 3, 4},
      {5, 6, 7, 8, 9},
      {10, 11, 12, 13, 14},
      {15, 16, 17, 18, 19},
  };
  Array2D<int> arr(il);

  bool res = arr.rows() == il.size() && arr.cols() == 5;
  if (!res) return false;

  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      if (i * arr.cols() + j != arr(i, j)) return false;
    }
  }

  return true;
}

bool TestInitContainerConstructor_array0() {
  int a[][5] = {
      {0, 1, 2, 3, 4},
      {5, 6, 7, 8, 9},
      {10, 11, 12, 13, 14},
      {15, 16, 17, 18, 19},
  };
  Array2D<int> arr(a);

  bool res = arr.rows() == std::extent_v<decltype(a), 0> && arr.cols() == std::extent_v<decltype(a), 1>;
  if (!res) return false;

  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      if (a[i][j] != arr(i, j)) return false;
    }
  }

  return true;
}

bool TestInitContainerConstructor_array1() {
  constexpr int rows = 512, cols = 512;
  int a[rows][cols];

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (auto &i : a) {
    for (int &j : i) {
      j = dist(engine);
    }
  }

  Array2D<int> arr(a);

  bool res = arr.rows() == rows && arr.cols() == cols;
  if (!res) return false;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (a[i][j] != arr(i, j)) return false;
    }
  }

  return true;
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
  if (!res) return false;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (a[i][j] != arr(i, j)) return false;
    }
  }

  return true;
}

bool TestInitContainerConstructor_set() {
  int rows = 512, cols = 512;
  std::set<int, std::greater<>> a;

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      a.insert(dist(engine));
    }
  }

  Array2D<int> arr(rows, cols, a);

  size_t i = 0;
  bool res = arr.rows() == rows && arr.cols() == cols;
  if (!res) return false;

  for (auto &&el : a) {
    size_t row = i / cols, col = i % cols;
    if (el != arr(row, col)) return false;
    ++i;
  }
  return true;
}

bool TestInitContainerConstructor_map() {
  int rows = 512, cols = 512;
  std::map<int, int> a;

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      a[i * cols + j] = dist(engine);
    }
  }

  Array2D<decltype(a)::value_type> arr(rows, cols, a);

  size_t i = 0;
  bool res = arr.rows() == rows && arr.cols() == cols;
  if (!res) return false;

  for (auto &&el : a) {
    size_t row = i / cols, col = i % cols;
    if (el != arr(row, col)) return false;
    ++i;
  }
  return true;
}

bool TestIterators() {
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
  const Array2D<int> carr(a);

  bool res = arr.rows() == rows && arr.cols() == cols;
  if (!res) return false;

  for (int i = 0; i < rows; ++i) {
    if (*arr.begin(i) != a[i][0] || *carr.cbegin(i) != a[i][0]) return false;
    if (*(arr.end(i) - 1) != a[i][cols - 1]
        || *(carr.cend(i) - 1) != a[i][cols - 1])
      return false;
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (a[i][j] != *arr.at(i * cols + j)) return false;
      if (a[i][j] != arr(i * cols + j)) return false;
    }
  }

  return true;
}

bool TestResizeAndAssign() {
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
  if (!res) return false;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (a[i][j] != arr(i, j)) return false;
    }
  }

  int val = dist(engine);
  arr.assign(rows, cols, val);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (val != arr(i, j)) return false;
    }
  }

  arr.resize(10, 10);
  if (arr.rows() != 10 || arr.cols() != 10) return false;

  return true;
}

int main() {
  Tester tester("Array2D Tests");

  tester
      .AddTest(TestRowsAndCols)
      .AddTest(TestInitContainerConstructor_initializer_list)
      .AddTest(TestInitContainerConstructor_array0)
      .AddTest(TestInitContainerConstructor_array1)
      .AddTest(TestInitContainerConstructor_vector)
      .AddTest(TestInitContainerConstructor_set)
      .AddTest(TestInitContainerConstructor_map)
      .AddTest(TestIterators)
      .AddTest(TestResizeAndAssign)
      .Run();

  return 0;
}