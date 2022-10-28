//
// Created by HimeHina on 2022/10/23.
//
#include <iostream>
#include <random>
#include <chrono>
#include <set>
#include <map>

#include "Utilities/Array3D.hpp"
#include "Tester.hpp"

bool TestRowsAndColsAndAisles() {
  constexpr const int rows = 329;
  constexpr const int cols = 932;
  constexpr const int aisles = 10;
  Array3D<int> arr(rows, cols, aisles, 9);
  return arr.rows() == rows && arr.cols() == cols && arr.aisles() == aisles;
}

bool TestInitContainerConstructor_initializer_list() {
  IList<IList<IList<int>>> il = {
      {
          {0, 1, 2, 3, 4},
          {5, 6, 7, 8, 9},
          {10, 11, 12, 13, 14},
          {15, 16, 17, 18, 19},
      }, {
          {20, 21, 22, 23, 24},
          {25, 26, 27, 28, 29},
          {30, 31, 32, 33, 34},
          {35, 36, 37, 38, 39},
      }, {
          {40, 41, 42, 43, 44},
          {45, 46, 47, 48, 49},
          {50, 51, 52, 53, 54},
          {55, 56, 57, 58, 59},
      }, {
          {60, 61, 62, 63, 64},
          {65, 66, 67, 68, 69},
          {70, 71, 72, 73, 74},
          {75, 76, 77, 78, 79},
      },
  };
  Array3D<int> arr(il);

  bool res = arr.rows() == il.size() && arr.cols() == 4 && arr.aisles() == 5;
  if (!res) return false;

  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      for (int k = 0; k < arr.aisles(); ++k) {
        if ((i * arr.cols() + j) * arr.aisles() + k != arr(i, j, k))
          return false;
      }
    }
  }

  return true;
}

bool TestInitContainerConstructor_array0() {
  int a[][4][5] = {
      {
          {0, 1, 2, 3, 4},
          {5, 6, 7, 8, 9},
          {10, 11, 12, 13, 14},
          {15, 16, 17, 18, 19},
      },
      {
          {20, 21, 22, 23, 24},
          {25, 26, 27, 28, 29},
          {30, 31, 32, 33, 34},
          {35, 36, 37, 38, 39},
      },
      {
          {40, 41, 42, 43, 44},
          {45, 46, 47, 48, 49},
          {50, 51, 52, 53, 54},
          {55, 56, 57, 58, 59},
      },
      {
          {60, 61, 62, 63, 64},
          {65, 66, 67, 68, 69},
          {70, 71, 72, 73, 74},
          {75, 76, 77, 78, 79},
      },
  };
  Array3D<int> arr(a);

  bool res = arr.rows() == std::extent_v<decltype(a), 0>
      && arr.cols() == std::extent_v<decltype(a), 1>
      && arr.aisles() == std::extent_v<decltype(a), 2>;
  if (!res) return false;
  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      for (int k = 0; k < arr.aisles(); ++k) {
        if (a[i][j][k] != arr(i, j, k)) return false;
      }
    }
  }
  return true;
}

bool TestInitContainerConstructor_array1() {
  constexpr int rows = 128, cols = 64, aisles = 32;
  int a[rows][cols][aisles];

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (auto &i : a) {
    for (auto &j : i) {
      for (auto &k : j) {
        k = dist(engine);
      }
    }
  }

  Array3D<int> arr(a);

  bool res = arr.rows() == rows && arr.cols() == cols && arr.aisles() == aisles;
  if (!res) return false;

  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      for (int k = 0; k < arr.aisles(); ++k) {
        if (a[i][j][k] != arr(i, j, k)) return false;
      }
    }
  }

  return true;
}

bool TestInitContainerConstructor_vector() {
  constexpr int rows = 512, cols = 512, aisles = 30;
  std::vector<std::vector<std::vector<int>>> a(
      rows,
      std::vector<std::vector<int>>(
          cols,
          std::vector<int>(
              aisles, 0
          )
      )
  );

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < aisles; ++k) {
        a[i][j][k] = dist(engine);
      }
    }
  }

  Array3D<int> arr(a);

  bool res = arr.rows() == rows && arr.cols() == cols && arr.aisles() == aisles;
  if (!res) return false;

  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      for (int k = 0; k < arr.aisles(); ++k) {
        if (a[i][j][k] != arr(i, j, k)) return false;
      }
    }
  }

  return true;
}

bool TestInitContainerConstructor_set() {
  constexpr int rows = 512, cols = 512, aisles = 30;
  std::set<int, std::greater<>> a;

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < aisles; ++k) {
        a.insert(dist(engine));
      }
    }
  }

  Array3D<int> arr(rows, cols, aisles, a);

  auto it = a.begin();
  bool res = arr.rows() == rows && arr.cols() == cols && arr.aisles() == aisles;
  if (!res) return false;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < aisles; ++k) {
        if (*(it++) != arr(i, j, k)) return false;
      }
    }
  }

  return res;
}

bool TestInitContainerConstructor_map() {
  constexpr int rows = 512, cols = 512, aisles = 30;
  std::map<int, int> a;

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < aisles; ++k) {
        a[(i * cols + j) * aisles + k] = dist(engine);
      }
    }
  }

  Array3D<decltype(a)::value_type> arr(rows, cols, aisles, a);

  auto it = a.begin();
  bool res = arr.rows() == rows && arr.cols() == cols && arr.aisles() == aisles;
  if (!res) return false;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < aisles; ++k) {
        if (*(it++) != arr(i, j, k)) return false;
      }
    }
  }

  return true;
}

bool TestIterators() {
  constexpr int rows = 512, cols = 512, aisles = 30;
  std::vector<std::vector<std::vector<int>>> a(
      rows,
      std::vector<std::vector<int>>(
          cols,
          std::vector<int>(
              aisles, 0
          )
      )
  );

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < aisles; ++k) {
        a[i][j][k] = dist(engine);
      }
    }
  }

  Array3D<int> arr(a);

  bool res = arr.rows() == rows && arr.cols() == cols && arr.aisles() == aisles;
  if (!res) return false;

  for (int i = 0; i < arr.rows(); ++i) {
    if (a[i][0][0] != *arr.begin(i)) return false;
  }
  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      if (a[i][j][0] != *arr.begin(i, j)) return false;
    }
  }
  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      for (int k = 0; k < arr.aisles(); ++k) {
        if (a[i][j][k] != arr((i * cols + j) * aisles + k)) return false;
      }
    }
  }

  return true;
}

bool TestResizeAndAssign() {
  constexpr int rows = 512, cols = 512, aisles = 30;
  std::vector<std::vector<std::vector<int>>> a(
      rows,
      std::vector<std::vector<int>>(
          cols,
          std::vector<int>(
              aisles, 0
          )
      )
  );

  std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < aisles; ++k) {
        a[i][j][k] = dist(engine);
      }
    }
  }

  Array3D<int> arr(a);

  bool res = arr.rows() == rows && arr.cols() == cols && arr.aisles() == aisles;
  if (!res) return false;

  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      for (int k = 0; k < arr.aisles(); ++k) {
        if (a[i][j][k] != arr(i, j, k)) return false;
      }
    }
  }

  int val = dist(engine);
  arr.assign(12, 52, 32, val);
  for (int i = 0; i < arr.rows(); ++i) {
    for (int j = 0; j < arr.cols(); ++j) {
      for (int k = 0; k < arr.aisles(); ++k) {
        if (val != arr(i, j, k)) return false;
      }
    }
  }

  arr.resize(10, 35, 29);
  res = arr.rows() == 10 && arr.cols() == 35 && arr.aisles() == 29;
  if (!res) return false;

  return true;
}

int main() {
  Tester tester("Array3D Tests");

  tester
      .AddTest(TestRowsAndColsAndAisles)
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