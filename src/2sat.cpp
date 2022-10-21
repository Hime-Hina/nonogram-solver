#include <iostream>
#include <unordered_map>
#include <vector>

class Graph {
  using Graph_ = std::vector<std::vector<int>>;

 public:
  Graph(size_t V_card) : _g(V_card + 1), _g_(V_card + 1) {
    // std::cout << "Graph with " << _g.size() - 1 << " nodes is constructed!\n";
  }

  void AddEdge(int u, int v) {
    _g[u].emplace_back(v);
    _g_[v].emplace_back(u);
  }

  const std::vector<int> &GetSCC() {
    _scc_counter = 0;
    _vis.assign(_g.size(), false);
    _scc.assign(_g.size(), _scc_counter);
    for (size_t u = 1; u < _g.size(); ++u) {
      if (!_vis[u]) _DFS1(u);
    }
    for (auto it = _post_orders.rbegin(), ie = _post_orders.rend(); it != ie;
         ++it) {
      if (_scc[*it] == 0) {
        ++_scc_counter;
        _DFS2(*it);
      }
    }
    return _scc;
  }

  ~Graph() {}

 private:
  size_t _scc_counter;
  std::vector<bool> _vis;
  std::vector<int> _scc, _post_orders;
  Graph_ _g, _g_;

  void _DFS1(int u) {
    _vis[u] = true;
    for (auto v : _g[u]) {
      if (!_vis[v]) _DFS1(v);
    }
    _post_orders.emplace_back(u);
  }

  void _DFS2(int u) {
    _scc[u] = _scc_counter;
    for (auto v : _g_[u]) {
      if (_scc[v] == 0) _DFS2(v);
    }
  }
};

bool Verify(std::vector<std::pair<int, int>> &cnf) {
  using namespace std;
  const int NOT = 1, NNOT = 2;

  unordered_map<int, bool> terms_cnt;
  for (auto p : cnf) {
    terms_cnt[abs(p.first)] = true;
    terms_cnt[abs(p.second)] = true;
  }
  // cout << "Total " << terms_cnt.size() << " terms\n";

  Graph graph(2 * terms_cnt.size());

  for (size_t i = 0; i < cnf.size(); ++i) {
    int p[2] = {cnf[i].first, cnf[i].second};
    if (p[0] > 0 && p[1] > 0) {  // x v y: !x -> y ^ !y -> x
      --p[0], --p[1];
      graph.AddEdge(NOT + 2 * p[0], NNOT + 2 * p[1]);
      graph.AddEdge(NOT + 2 * p[1], NNOT + 2 * p[0]);
    } else if (p[0] > 0 && p[1] < 0) {  // x v !y: !x -> !y ^ y -> x
      --p[0], p[1] = -p[1] - 1;
      graph.AddEdge(NOT + 2 * p[0], NOT + 2 * p[1]);
      graph.AddEdge(NNOT + 2 * p[1], NNOT + 2 * p[0]);
    } else if (p[0] < 0 && p[1] < 0) {  // !x v !y: x -> !y ^ y -> !x
      p[0] = -p[0] - 1, p[1] = -p[1] - 1;
      graph.AddEdge(NNOT + 2 * p[0], NOT + 2 * p[1]);
      graph.AddEdge(NNOT + 2 * p[1], NOT + 2 * p[0]);
    } else if (p[0] < 0 && p[1] > 0) {  // !x v y: x -> y ^ !y -> !x
      p[0] = -p[0] - 1, --p[1];
      graph.AddEdge(NNOT + 2 * p[0], NNOT + 2 * p[1]);
      graph.AddEdge(NOT + 2 * p[1], NOT + 2 * p[0]);
    }
  }
  auto scc = graph.GetSCC();
  for (size_t i = 1; i < scc.size(); i += 2) {
    if (scc[i] == scc[i + 1]) return false;
  }

  return true;
}

int main() {
  using namespace std;

  int len_cnf;
  cin >> len_cnf;

  vector<pair<int, int>> cnf(len_cnf);

  for (int i = 0; i < len_cnf; ++i) {
    cin >> cnf[i].first >> cnf[i].second;
  }

  if (Verify(cnf)) {
    cout << "Satisifiable\n";
  } else {
    cout << "Unsatisifiable\n";
  }

  return 0;
}