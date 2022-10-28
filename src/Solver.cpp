//
// Created by HimeHina on 2022/10/23.
//

#include <initializer_list>
#include <algorithm>

#include "Solver.hpp"

bool Solver::Solve() {
  try {
    Initialize_();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }

  Timer t;
  FillSimpleBlocks_();
  if (DFS_(0)) {
    std::cout << "A solution is found!\n";
    return true;
  } else {
    std::cout << "No solution is found!\n";
    return false;
  }
}

void Solver::Print(bool show_descriptions, std::ostream &out) {
  InitializeForPrinting_(show_descriptions);
  // output to terminal
  for (int i = 0; i < gram_for_printing_.rows(); ++i) {
    for (int j = 0; j < gram_for_printing_.cols(); ++j) {
      out << std::setw(2) << gram_for_printing_(i, j);
    }
    out << '\n';
  }
}

std::istream &operator>>(std::istream &in, Solver &solver) {
  std::string input_line;
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    for (int i = 1; i <= solver.board_size_[line]; ++i) {
      int k;
      auto begin = solver.descriptions_.begin(line, i),
          end = solver.descriptions_.end(line, i);
      while (getline(in, input_line),
          (k = Solver::ParseLine_(input_line, begin, end, 1)) <= 0);
      solver.descriptions_(line, i, 0) = k;
      if (2 * k > solver.board_size_[1 - line] + 1)
        throw Solver::IncorrectDescriptions();
    }
  }
  std::cout << "I get a " << solver.board_size_[Line::ROW] << " X "
            << solver.board_size_[Line::COL] << " nonogram." << std::endl;

  return in;
}

std::ostream &operator<<(std::ostream &out, Solver &solver) {
  solver.Print(true, out);
  return out;
}

template <
    typename T, std::enable_if_t<is_iterator_v<T>, bool>
>
int Solver::ParseLine_(std::string l, T begin, T end, std::size_t assign_from) {
  if (l.empty()) return 0;
  if (assign_from > 0) begin += assign_from - 1;
  if (begin >= end) return 0;

  int n = 0;
  int k = 0;

  for (auto it = l.begin(),
           ie = l.end(); it != ie; ++it) {
    if (isdigit(*it)) {
      n = 10 * n + (*it - '0');
      if (it + 1 == ie || !isdigit(*(it + 1))) {
        *(++begin) = n;
        ++k;
        if (begin >= end) break;
      }
    } else
      n = 0;
  }
  return k;
}

void Solver::InitializeForPrinting_(bool show_descriptions) {
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
  std::size_t total_row, total_col;
  if (show_descriptions) {
    total_row = board_size_[Line::ROW] + max_description_lens_[Line::COL];
    total_col = board_size_[Line::COL] + max_description_lens_[Line::ROW];
  } else {
    total_row = board_size_[Line::ROW] + 1;
    total_col = board_size_[Line::COL] + 1;
  }
  gram_for_printing_.assign(total_row, total_col, ' ');

  if (show_descriptions) {
    // set area 2
    for (int i = 1; i <= board_size_[Line::COL]; ++i) {
      for (int j = 1; j <= descriptions_(Line::COL, i, 0); ++j) {
        int to_row = max_description_lens_[Line::COL] -
                     descriptions_(Line::COL, i, 0) + j - 1,
            to_col = max_description_lens_[Line::ROW] + i - 1;
        gram_for_printing_(to_row, to_col) =
            _description_number_digits[descriptions_(Line::COL, i, j)];
      }
    }

    // set area 3
    for (int i = 1; i <= board_size_[Line::ROW]; ++i) {
      for (int j = 1; j <= descriptions_(Line::ROW, i, 0); ++j) {
        int to_row = max_description_lens_[Line::COL] + i - 1,
            to_col = max_description_lens_[Line::ROW] -
                     descriptions_(Line::ROW, i, 0) + j - 1;
        gram_for_printing_(to_row, to_col) =
            _description_number_digits[descriptions_(Line::ROW, i, j)];
      }
    }
  } else {
    // set area 2
    for (int i = 1; i <= board_size_[Line::COL]; ++i) {
      gram_for_printing_(0, i) = _description_number_digits[i];
    }
    // set area 3
    for (int i = 1; i <= board_size_[Line::ROW]; ++i) {
      gram_for_printing_(i, 0) = _description_number_digits[i];
    }
  }

  // set area 4
  for (int i = 1; i <= board_size_[Line::ROW]; ++i) {
    for (int j = 1; j <= board_size_[Line::COL]; ++j) {
      int to_row, to_col;
      if (show_descriptions) {
        to_row = max_description_lens_[Line::COL] + i - 1,
            to_col = max_description_lens_[Line::ROW] + j - 1;
      } else {
        to_row = i, to_col = j;
      }
      gram_for_printing_(to_row, to_col) = block_chars_[gram_(i, j)];
    }
  }
}

void Solver::Initialize_() {
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    sum_line_sum_[line] = 0;
    for (int i = 1; i <= board_size_[line]; ++i) {
      max_description_lens_[line] = std::max(max_description_lens_[line], descriptions_(line, i, 0));
      InitializeAt_(line, i);
    }
  }

  if (sum_line_sum_[Line::ROW] == 0 && sum_line_sum_[Line::COL] == 0)
    throw EmptyDescriptions();
  if (sum_line_sum_[Line::ROW] != sum_line_sum_[Line::COL])
    throw IncorrectDescriptions();
}

void Solver::InitializeAt_(std::size_t line, std::size_t index) {
  std::size_t k = 2 * descriptions_(line, index, 0) + 1;

  int len = descriptions_(line, index, 0);
  line_sum_(line, index) = 0;
  line_upper_sum_(line, index) = len - 1;

  for (int j = 1; j <= len; ++j) {
    line_sum_(line, index) += descriptions_(line, index, j);
    sum_line_sum_[line] += descriptions_(line, index, j);

    if (line_sum_(line, index) > board_size_[1 - line])
      throw Solver::IncorrectDescriptions();
  }
  line_upper_sum_(line, index) += line_sum_(line, index);

  offset_(line, index) = (int) board_size_[1 - line] - line_upper_sum_(line, index);

  // initialize a_ and b_
  a_(line, index, 0) = 0;
  a_(line, index, 1) = 0;
  b_(line, index, 0) = 0;
  b_(line, index, 1) = offset_(line, index);
  for (int j = 2; j < k; ++j) {
    a_(line, index, j) =
        j & 1 ? 1 : descriptions_(line, index, j / 2);
    b_(line, index, j) =
        j & 1 ? offset_(line, index) + 1 : descriptions_(line, index, j / 2);
  }
  a_(line, index, k) = 0;
  b_(line, index, k) = offset_(line, index);

  // compute A_ and B_
  A_(line, index, 0) = 0;
  B_(line, index, 0) = 0;
  for (int j = 1; j <= k; ++j) {
    A_(line, index, j) =
        A_(line, index, j - 1)
        + a_(line, index, j);
    B_(line, index, j) =
        B_(line, index, j - 1)
        + b_(line, index, j);
  }
}

void Solver::FillSimpleBlocks_() {
  std::cout << "Pre-filling...\n";
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    for (int i = 1; i <= board_size_[line]; ++i) {
      int begin = offset_(line, i) + 1, end = 1;
      for (int j = 1; j <= descriptions_(line, i, 0); ++j) {
        end += descriptions_(line, i, j);
        if (descriptions_(line, i, j) > offset_(line, i)) {
          for (int k = begin; k < end; ++k) {
            if (line == Line::ROW) {
              gram_(i, k) = Block::BLACK;
            } else if (line == Line::COL) {
              gram_(k, i) = Block::BLACK;
            }
          }
        }
        end += 1;
        begin = end + offset_(line, i);
      }
    }
  }

  // Marking blocks that are in 0-clue's line as blank
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    for (int i = 1; i <= board_size_[line]; ++i) {
      if (line_sum_(line, i) == 0) {
        for (int j = 1; j <= board_size_[1 - line]; ++j) {
          if (line == Line::ROW)
            gram_(i, j) = Block::BLANK;
          else if (line == Line::COL)
            gram_(j, i) = Block::BLANK;
        }
      }
    }
  }

  // Get unknown blocks
  for (int i = 1; i <= board_size_[Line::ROW]; ++i) {
    for (int j = 1; j <= board_size_[Line::COL]; ++j) {
      if (gram_(i, j) == Block::UNKNOWN) {
        unknown_pos_.emplace_back(Pos{i, j});
      }
    }
  }

  std::cout << "Total " << total_blocks_ - unknown_pos_.size()
            << " blocks are pre-filled, "
            << "and " << unknown_pos_.size()
            << " blocks remain to be filled.\n";
}

bool Solver::IsFixable_(Array2D<bool> &is_fix, std::size_t line, std::size_t index) {
  if (line != Line::ROW && line != Line::COL) return false;
  if (index > board_size_[line]) return false;

  int l = (int) board_size_[1 - line];
  int k = 2 * descriptions_(line, index, 0) + 1;

  is_fix.assign(k + 1, l + 1, false);
  for (int i = 0; i <= k; ++i) { // initial state
    if (A_(line, index, i) == 0) {
      is_fix(i, 0) = true;
    }
  }

  // Compute L_{i}^{\sigma_j}(s)
  int index_keeper[Block::LOOP_END] = {0, 0};
  Array2D<int> L(Block::LOOP_END, l + 1);
  for (int i = 0; i <= l; ++i) {
    int block_type;
    block_type = line == Line::ROW ? gram_(index, i) : gram_(i, index);
    if (block_type != Block::UNKNOWN) {
      index_keeper[block_type] = i;
    }
    L(Block::BLANK, i) = index_keeper[Block::BLACK];
    L(Block::BLACK, i) = index_keeper[Block::BLANK];
  }

  int sigma = Block::BLANK;
  for (int j = 1; j <= k; ++j) {
    int begin_i = std::max(1, A_(line, index, j)),
        end_i = std::min(l, B_(line, index, j)) + 1;

    for (int i = begin_i; i < end_i; ++i) {
      int begin_p = std::max(
          {
              i - b_(line, index, j),
              A_(line, index, j - 1),
              L(sigma, i)
          }
      );
      int end_p = std::min(
          i - a_(line, index, j),
          B_(line, index, j - 1)
      ) + 1;

      for (int p = begin_p; p < end_p; ++p) {
        if (is_fix(j - 1, p)) {
          is_fix(j, i) = true;
          break;
        }
      }
    }
    sigma = 1 - sigma;
  }

  return is_fix(k, l);
}

bool Solver::Check_(Pos pos) {
  int line2index[2] = {pos.row, pos.col};
  std::size_t max_size = std::max(board_size_[0], board_size_[1]);
  std::size_t max_len = std::max(max_description_lens_[0], max_description_lens_[1]);

  Array2D<bool> fix;
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    if (!IsFixable_(fix, line, line2index[line])) return false;
  }

  return true;
}

bool Solver::DFS_(int index) {
  if (index == unknown_pos_.size()) return true;

  Pos cur = unknown_pos_[index];
  for (int i = Block::BLANK; i < Block::LOOP_END; ++i) {
    gram_(cur.row, cur.col) = i;
    if (Check_(cur) and DFS_(index + 1)) return true;
  }
  gram_(cur.row, cur.col) = Block::UNKNOWN;

  return false;
}
