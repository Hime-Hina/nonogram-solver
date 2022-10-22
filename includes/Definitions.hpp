//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_DEFINITIONS_HPP_
#define NONOGRAM_INCLUDES_DEFINITIONS_HPP_

#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

#include "Graph/Edge.hpp"

#define ITERATE_VERTICES_TMP(u, tot) for (int u = 0; u < (tot); ++u)

#define ITERATE_VERTICES(u, tot) for (u = 0; u < (tot); ++u)

#define ITERATE_EDGES(e, edges) for (auto &&e : (edges))

constexpr const int inf = 0x3f3f3f3f;

template <typename T, typename Category = std::random_access_iterator_tag>
using Iterator = std::iterator<Category, T>;
template <typename T, typename Category = std::random_access_iterator_tag>
using IteratorRange = std::pair<Iterator<T, Category>, Iterator<T, Category>>;

template <typename T>
using Array = std::vector<T>;
template <typename U>
using IList = std::initializer_list<U>;

using Flags = std::vector<bool>;
using iArray = std::vector<int>;
using szArray = std::vector<std::size_t>;
using iStack = std::stack<int>;
using iQueue = std::queue<int>;
using iMapi = std::unordered_map<int, int>;
using iiMapi = std::unordered_map<int, std::unordered_map<int, int>>;
using EdgeSet = std::vector<Graph::Edge>;
using AdjList = std::vector<EdgeSet>;

#endif //NONOGRAM_INCLUDES_GRAPH_DEFINITIONS_HPP_
