#include <iostream>
#include <string>
#include <vector>

static bool _ParseLine(std::string l, std::vector<int> &arr,
                       size_t index_start = 0) {
  int n = 0;
  arr.assign(index_start, 0);
  for (auto it = l.begin(), ie = l.end(); it != ie; ++it) {
    if (isdigit(*it)) {
      n = 10 * n + (*it - '0');
      if (it + 1 == ie || !isdigit(*(it + 1))) {
        arr.emplace_back(n);
      }
    } else
      n = 0;
  }
  return arr.size() != index_start;
}

int main() {
  using namespace std;

  int board_size[2];

  cin >> board_size[0] >> board_size[1];
  vector<vector<bool>> board(board_size[0]);
  for (int i = 0; i < board_size[0]; ++i) {
    board[i].assign(board_size[1], false);
  }

  string l;
  vector<vector<int>> descriptions[2], descriptions_given[2];
  for (int line = 0; line < 2; ++line) {
    for (int i = 0; i < board_size[line]; ++i) {
      while (getline(std::cin, l), !_ParseLine(l, descriptions_given[line][i]));
    }
  }
  for (int i = 0; i < board_size[0]; ++i) {
    for (int j = 0; j < board_size[1]; ++j) {
      bool b;
      cin >> b;
      board[i][j] = b;
    }
  }

  return 0;
}