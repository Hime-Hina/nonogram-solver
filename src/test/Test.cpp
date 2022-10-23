//
// Created by HimeHina on 2022/10/24.
//
#include <iostream>
#include <type_traits>

int main() {
  int a[][5][4] = {{{}, {}}, {}, {}};
  std::cout << std::extent_v<decltype(a), 0> << std::endl;
  std::cout << std::extent_v<decltype(a), 1> << std::endl;
  std::cout << std::extent_v<decltype(a), 2> << std::endl;
  std::cout << std::extent_v<decltype(a), 3> << std::endl;

  return 0;
}