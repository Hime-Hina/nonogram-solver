#include <algorithm>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <initializer_list>


template <typename T>
class Array2D {
  template <typename U>
  using IList = std::initializer_list<U>;

 public:
  Array2D(size_t rows, size_t cols)
      : _rows(rows), _cols(cols), _arr(rows * cols) {}
  Array2D(size_t rows, size_t cols, const T &val)
      : _rows(rows), _cols(cols), _arr(rows * cols, val) {}
  Array2D(const Array2D &arr) = default;
  Array2D(Array2D &&arr) = default;
  Array2D(IList<IList<T>> list) : _rows(list.size()) {
    _cols = 0;
    for (auto &l : list) {
      _cols = std::max(_cols, l.size());
    }
    _arr.resize(_rows * _cols);
    for (auto lit = list.begin(), lie = list.end(), ait = _arr.begin();
         lit != lie; ++lit, ait += _cols) {
      std::copy(lit->begin(), lit->end(), ait);
    }
  }

  Array2D &operator=(const Array2D &arr) {
    _rows = arr._rows;
    _cols = arr._cols;
    _arr = arr._arr;
    return *this;
  }

  size_t rows() const { return _rows; }
  size_t cols() const { return _cols; }
  T &operator()(size_t row, size_t col) { return _arr[row * _cols + col]; }
  const T operator()(size_t row, size_t col) const {
    return _arr[row * _cols + col];
  }

 private:
  size_t _rows, _cols;
  std::vector<T> _arr;
};

template <typename T>
class Array3D {
  template <typename U>
  using IList = std::initializer_list<U>;

 public:
  Array3D(size_t rows, size_t cols, size_t aisles)
      : _rows(rows),
        _cols(cols),
        _aisles(aisles),
        _offset(cols * aisles),
        _arr(rows * cols * aisles) {}
  Array3D(size_t rows, size_t cols, size_t aisles, const T &val)
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
  Array3D(Array3D &&arr)
      : _rows(arr._rows),
        _cols(arr._cols),
        _aisles(arr._aisles),
        _offset(arr._cols * arr._aisles),
        _arr(arr._arr) {}
  Array3D(IList<IList<IList<T>>> list) : _rows(list.size()) {
    _cols = 0;
    _aisles = 0;
    for (auto &l : list) {
      _cols = std::max(_cols, l.size());
      for (auto &ll : l) {
        _aisles = std::max(_aisles, ll.size());
      }
    }
    _offset = _cols * _aisles;

    _arr.resize(_rows * _cols * _aisles);
    for (auto lit = list.begin(), lie = list.end(), ait = _arr.begin();
         lit != lie; ++lit, ait += _offset) {
      for (auto llit = lit->begin(), llie = lit->end(), aiit = ait;
           llit != llie; ++llit, aiit += _aisles) {
        std::copy(llit->begin(), llit->end(), aiit);
      }
    }
  }

  Array3D &operator=(const Array3D &arr) {
    _rows = arr._rows;
    _cols = arr._cols;
    _aisles = arr._aisles;
    _offset = arr._offset;
    _arr = arr._arr;
    return *this;
  }
  Array3D &operator=(Array3D &&arr) {
    _rows = arr._rows;
    _cols = arr._cols;
    _aisles = arr._aisles;
    _offset = arr._offset;
    _arr = arr._arr;
    return *this;
  }

  size_t rows() const { return _rows; }
  size_t cols() const { return _cols; }
  size_t aisle() const { return _aisles; }
  T &operator()(size_t row, size_t col, size_t aisle) {
    return _arr[row * _offset + col * _aisles + aisle];
  }
  const T operator()(size_t row, size_t col, size_t aisle) const {
    return _arr[row * _offset + col * _aisles + aisle];
  }

 private:
  size_t _rows, _cols, _aisles;
  size_t _offset;
  std::vector<T> _arr;
};

template <size_t MAX_SIZE = 100ULL>
class Nonogram {
 public:
  struct Line {
    enum { ROW = 0, COL, LOOP_END };
  };
  struct Block {
    enum { BLANK = 0, BLACK, LOOP_END, UNKNOWN = 2 };
  };
  struct Pos {
    int row, col;
  };

  Nonogram() {}

  bool Solve() {
    if (!_Initialize()) {
      std::cout
          << "The sum of row's sum does not equal to the sum of col's sum!\n";
      std::cout << _sum_line_sum[0] << " != " << _sum_line_sum[1] << '\n';
      return false;
    }

    Timer t;
    _FillSimpleBlocks();
    if (_DFS(0)) {
      std::cout << "A solution is found!\n";
      return true;
    } else {
      std::cout << "No solution is found!\n";
      return false;
    }
  }

  void Input() {
    std::cin >> _board_size[Line::ROW] >> _board_size[Line::COL];
    _total_blocks = _board_size[Line::ROW] * _board_size[Line::COL];

    std::string input_line;
    for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
      _sum_line_sum[line] = 0;

      for (int i = 1; i <= _board_size[line]; ++i) {
        while (getline(std::cin, input_line),
               !_ParseLine(input_line, 1, _descriptions[line][i],
                           _description_lens[line] + i))
          ;
        _line_sum[line][i] = 0;
        _line_upper_sum[line][i] = _description_lens[line][i] - 1;
        _max_description_lens[line] =
            std::max(_max_description_lens[line], _description_lens[line][i]);

        for (int j = 1; j <= _description_lens[line][i]; ++j) {
          _line_sum[line][i] += _descriptions[line][i][j];
          _sum_line_sum[line] += _descriptions[line][i][j];
        }
        _line_upper_sum[line][i] += _line_sum[line][i];
      }
    }
    std::cout << "I get a " << _board_size[Line::ROW] << " X "
              << _board_size[Line::COL] << " nonogram.\n";
  }
  void Print(bool show_descriptions = true) {
    /* the gram for printing consists of these areas:
      +-----------+
      |1|    2    |
      |-+---------|
      | |         |
      |3|    4    |
      | |         |
      +-----------+
      1: blank
      2: the descriptions of columns
      3: the descriptions of rows
      4: the main board of nonogram
    */
    // initialize
    size_t total_row, total_col;
    if (show_descriptions) {
      total_row = _board_size[Line::ROW] + _max_description_lens[Line::COL];
      total_col = _board_size[Line::COL] + _max_description_lens[Line::ROW];
    } else {
      total_row = _board_size[Line::ROW] + 1;
      total_col = _board_size[Line::COL] + 1;
    }
    for (int row = 0; row < total_row; ++row) {
      for (int col = 0; col < total_col; ++col) {
        _gram_for_printing[row][col] = ' ';
      }
    }

    if (show_descriptions) {
      // set area 2
      for (int i = 1; i <= _board_size[Line::COL]; ++i) {
        for (int j = 1; j <= _description_lens[Line::COL][i]; ++j) {
          int to_row = _max_description_lens[Line::COL] -
                       _description_lens[Line::COL][i] + j - 1,
              to_col = _max_description_lens[Line::ROW] + i - 1;
          _gram_for_printing[to_row][to_col] =
              _description_number_digits[_descriptions[Line::COL][i][j]];
        }
      }

      // set area 3
      for (int i = 1; i <= _board_size[Line::ROW]; ++i) {
        for (int j = 1; j <= _description_lens[Line::ROW][i]; ++j) {
          int to_row = _max_description_lens[Line::COL] + i - 1,
              to_col = _max_description_lens[Line::ROW] -
                       _description_lens[Line::ROW][i] + j - 1;
          _gram_for_printing[to_row][to_col] =
              _description_number_digits[_descriptions[Line::ROW][i][j]];
        }
      }
    } else {
      // set area 2
      for (int i = 1; i <= _board_size[Line::COL]; ++i) {
        _gram_for_printing[0][i] = _description_number_digits[i];
      }
      // set area 3
      for (int i = 1; i <= _board_size[Line::ROW]; ++i) {
        _gram_for_printing[i][0] = _description_number_digits[i];
      }
    }

    // set area 4
    for (int i = 1; i <= _board_size[Line::ROW]; ++i) {
      for (int j = 1; j <= _board_size[Line::COL]; ++j) {
        int to_row, to_col;
        if (show_descriptions) {
          to_row = _max_description_lens[Line::COL] + i - 1,
          to_col = _max_description_lens[Line::ROW] + j - 1;
        } else {
          to_row = i, to_col = j;
        }
        _gram_for_printing[to_row][to_col] = _block_chars[_gram[i][j]];
      }
    }

    // output to terminal
    for (int i = 0; i < total_row; ++i) {
      for (int j = 0; j < total_col; ++j) {
        std::cout << std::setw(2) << _gram_for_printing[i][j];
      }
      std::cout << '\n';
    }
    std::cout << '\n';

    // for (int i = 0; i <= _board_size[Line::ROW]; ++i) {
    //   for (int j = 0; j <= _board_size[Line::COL]; ++j) {
    //     std::cout << std::setw(2) << _gram[i][j] << ' ';
    //   }
    //   std::cout << '\n';
    // }

    // for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    //   std::cout << (line == Line::ROW ? "Row:" : "Col:") << '\n';
    //   for (int i = 1; i <= _board_size[line]; ++i) {
    //     std::cout << "A ";
    //     for (int j = 0; j <= 2 * _description_lens[line][i] + 1; ++j) {
    //       std::cout << _A[line][i][j] << ' ';
    //     }
    //     std::cout << '\n';
    //     std::cout << "B ";
    //     for (int j = 0; j <= 2 * _description_lens[line][i] + 1; ++j) {
    //       std::cout << _B[line][i][j] << ' ';
    //     }
    //     std::cout << '\n';
    //   }
    //   std::cout << '\n';
    // }
  }

  ~Nonogram() {}

 private:
  constexpr static const char *_description_number_digits =
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  size_t _total_blocks;
  size_t _board_size[Line::LOOP_END];

  Block _gram[MAX_SIZE + 1][MAX_SIZE + 1];  // 1-indexed
  char _block_chars[Block::LOOP_END + 1] = {'.', '@', '?'};
  char _gram_for_printing[MAX_SIZE + (MAX_SIZE + 1) / 2]
                         [MAX_SIZE + (MAX_SIZE + 1) / 2];

  int _max_description_lens[Line::LOOP_END],
      _description_lens[Line::LOOP_END][MAX_SIZE + 1];  // 1-indexed
  // the descriptions of a nonogram always are thie formal:
  // 0{0,offset}1{d_1}0{1,offset+1}1{d_2}0{1,offset+1}...0{1,offset+1}1{d_k}0{0,offset},
  // where offset = _board_size[line] - (k - 1 + \sum_{i=1}^{k} d_i)
  int _descriptions[Line::LOOP_END][MAX_SIZE + (MAX_SIZE + 1) / 2 + 1]
                   [MAX_SIZE + (MAX_SIZE + 1) / 2 + 1];  // 1-indexed

  // for description d_1 d_2 ... d_k, its line upper sum is
  // k - 1 + linesum_i = k - 1 + \sum_{i=1}^{k} d_i
  int _line_sum[Line::LOOP_END][MAX_SIZE + 1];        // 1-indexed
  int _line_upper_sum[Line::LOOP_END][MAX_SIZE + 1];  // 1-indexed
  int _sum_line_sum[Line::LOOP_END];
  // offset_i is board_size - line_upper_sum_i,
  // if line_upper_sum_i is about row, board_size is the size of column;
  // otherwise is the size of row
  int _offset[Line::LOOP_END][MAX_SIZE + 1];  // 1-indexed

  // the length of the 3rd dimension can be determined with 2 * k + 1, where k =
  // floor((size + 1) / 2), so 2 * k + 1 <= size + 2.
  // then insert a A_0 to _A or a B_0 to _B, finally, the length is size + 3
  int _a[Line::LOOP_END][MAX_SIZE + 1][MAX_SIZE + 3];
  int _b[Line::LOOP_END][MAX_SIZE + 1][MAX_SIZE + 3];
  // prefix sum of a_i
  int _A[Line::LOOP_END][MAX_SIZE + 1][MAX_SIZE + 3];
  // ............. b_i
  int _B[Line::LOOP_END][MAX_SIZE + 1][MAX_SIZE + 3];
  int _L[Block::LOOP_END][MAX_SIZE + 1];
  // if the definition of Fix is Fix(s^(i),d^(j)) instead Fix(d^(j),s^(i)),
  // we will get a lot of cache miss...
  bool _fix[MAX_SIZE + 3][MAX_SIZE];

  size_t _total_unknown_pos = 0;
  Pos _unknown_pos[MAX_SIZE * MAX_SIZE];

  static bool _ParseLine(std::string l, size_t index_start, int *arr, int *k) {
    int n = 0;
    *k = 0;
    for (auto it = l.begin(), ie = l.end(); it != ie; ++it) {
      if (isdigit(*it)) {
        n = 10 * n + (*it - '0');
        if (it + 1 == ie || !isdigit(*(it + 1))) {
          arr[index_start + (*k)++] = n;
        }
      } else
        n = 0;
    }
    return (*k) != 0;
  }

  bool _Initialize() {
    for (int i = 0; i <= _board_size[Line::ROW]; ++i) {
      for (int j = 0; j <= _board_size[Line::COL]; ++j) {
        _gram[i][j] = Block::UNKNOWN;
      }
    }
    // initialize _a and _b
    for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
      for (int i = 1; i <= _board_size[line]; ++i) {
        _offset[line][i] = _board_size[1 - line] - _line_upper_sum[line][i];
        _a[line][i][0] = 0;
        _a[line][i][1] = 0;
        _b[line][i][0] = 0;
        _b[line][i][1] = _offset[line][i];
        for (int j = 2; j < 2 * _description_lens[line][i] + 1; ++j) {
          _a[line][i][j] = j & 1 ? 1 : _descriptions[line][i][j / 2];
          _b[line][i][j] =
              j & 1 ? _offset[line][i] + 1 : _descriptions[line][i][j / 2];
        }
        _a[line][i][2 * _description_lens[line][i] + 1] = 0;
        _b[line][i][2 * _description_lens[line][i] + 1] = _offset[line][i];
      }
    }
    // compute _A and _B
    for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
      for (int i = 1; i <= _board_size[line]; ++i) {
        _A[line][i][0] = 0;
        _B[line][i][0] = 0;
        for (int j = 1; j <= 2 * _description_lens[line][i] + 1; ++j) {
          _A[line][i][j] = _A[line][i][j - 1] + _a[line][i][j];
          _B[line][i][j] = _B[line][i][j - 1] + _b[line][i][j];
        }
      }
    }
    return _sum_line_sum[Line::ROW] == _sum_line_sum[Line::COL];
  }

  void _FillSimpleBlocks() {
    std::cout << "Pre-filling...\n";
    for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
      for (int i = 1; i <= _board_size[line]; ++i) {
        int start = _offset[line][i] + 1, end = 1;
        for (int j = 1; j <= _description_lens[line][i]; ++j) {
          end += _descriptions[line][i][j];
          if (_descriptions[line][i][j] > _offset[line][i]) {
            for (int k = start; k < end; ++k) {
              if (line == Line::ROW) {
                _gram[i][k] = Block::BLACK;
              } else if (line == Line::COL) {
                _gram[k][i] = Block::BLACK;
              }
            }
          }
          // start = end + offset + 1
          end += 1;
          start = end +
                  _offset[line][i];  // or start += descriptions[line][i][j] + 1
        }
      }
    }

    // Marking blocks that are in 0-clue's line as blank
    for (int i = 1; i <= _board_size[Line::ROW]; ++i) {
      if (_line_upper_sum[Line::ROW][i] == 0) {
        for (int j = 1; j <= _board_size[Line::COL]; ++j) {
          _gram[i][j] = Block::BLANK;
        }
      }
    }
    for (int j = 1; j <= _board_size[Line::COL]; ++j) {
      if (_line_upper_sum[Line::COL][j] == 0) {
        for (int i = 1; i <= _board_size[Line::ROW]; ++i) {
          _gram[i][j] = Block::BLANK;
        }
      }
    }
    for (int i = 1; i <= _board_size[Line::ROW]; ++i) {
      for (int j = 1; j <= _board_size[Line::COL]; ++j) {
        if (_gram[i][j] == Block::UNKNOWN) {
          _unknown_pos[_total_unknown_pos++] = Pos{i, j};
        }
      }
    }
    // std::shuffle(_unknown_pos, _unknown_pos + _total_unknown_pos,
    //         std::default_random_engine(std::chrono::high_resolution_clock::now()
    //                                        .time_since_epoch()
    //                                        .count()));

    std::cout << "Total " << _total_blocks - _total_unknown_pos
              << " blocks are pre-filled, "
              << "and " << _total_unknown_pos
              << " blocks remain to be filled.\n";
    // for (int i = 0; i < _total_unknown_pos; ++i) {
    //   std::cout << _unknown_pos[i].row << ' ' << _unknown_pos[i].col << '\n';
    // }
  }

  bool _Check(Pos pos) {
    int pp[2] = {pos.row, pos.col};

    for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
      int l = _board_size[1 - line];
      int k = 2 * _description_lens[line][pp[line]] + 1;
      int h[Block::LOOP_END] = {0, 0};

      for (int i = 0; i <= l; ++i) {
        Block block_type;
        if (line == Line::ROW) {
          block_type = _gram[pp[line]][i];
        } else if (line == Line::COL) {
          block_type = _gram[i][pp[line]];
        }
        if (block_type != Block::UNKNOWN) {
          h[block_type] = i;
        }
        _L[Block::BLANK][i] = h[Block::BLACK];
        _L[Block::BLACK][i] = h[Block::BLANK];
      }
      for (int j = 0; j <= k; ++j) {
        for (int i = 0; i <= l; ++i) {
          _fix[j][i] = false;
        }
      }
      for (int i = 0; i <= k; ++i) {
        if (_A[line][pp[line]][i] == 0) {
          _fix[i][0] = true;
        }
      }

      int sigma = Block::BLANK;
      for (int j = 1; j <= k; ++j) {
        int i_from = std::max(1, _A[line][pp[line]][j]),
            i_to = std::min(l, _B[line][pp[line]][j]);
        for (int i = i_from; i <= i_to; ++i) {
          int p_from =
                  std::max(i - _b[line][pp[line]][j],
                           std::max(_A[line][pp[line]][j - 1], _L[sigma][i])),
              p_to = std::min(i - _a[line][pp[line]][j],
                              _B[line][pp[line]][j - 1]);
          for (int p = p_from; p <= p_to; ++p) {
            if (_fix[j - 1][p]) {
              _fix[j][i] = true;
              break;
            }
          }
        }
        sigma = 1 - sigma;
      }

      if (!_fix[k][l]) return false;
    }

    return true;
  }

  bool _DFS(int index) {
    if (index == _total_unknown_pos) return true;

    Pos cur = _unknown_pos[index];
    for (int i = Block::BLANK; i < Block::LOOP_END; ++i) {
      _gram[cur.row][cur.col] = Block(i);
      if (_Check(cur) and _DFS(index + 1)) return true;
    }
    _gram[cur.row][cur.col] = Block::UNKNOWN;

    return false;
  }
};

int main() {
  Nonogram<> nonogram;

  nonogram.Input();
  nonogram.Solve();
  nonogram.Print(false);

  return 0;
}
