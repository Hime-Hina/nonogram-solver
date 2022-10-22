//
// Created by HimeHina on 2022/10/23.
//

#include "Solver.hpp"

bool Solver::Solve() {
  if (!Initialize_()) {
    std::cout
        << "The sum of row's sum does not equal to the sum of col's sum!\n";
    std::cout << sum_line_sum_[0] << " != " << sum_line_sum_[1] << '\n';
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

void Solver::Input() {
//    std::cin >> board_size_[Line::ROW] >> board_size_[Line::COL];
//    total_blocks_ = board_size_[Line::ROW] * board_size_[Line::COL];

  std::string input_line;
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    sum_line_sum_[line] = 0;

    for (int i = 1; i <= board_size_[line]; ++i) {
      while (
          getline(std::cin, input_line),
              (
                  descriptions_(line, i, 0) =
                      ParseLine_<int>(
                          input_line,
                          descriptions_(line, i),
                          descriptions_.end(line, i),
                          1
                      )
              ) > 0
          );
      int k = descriptions_(line, i, 0);
      line_sum_(line, i) = 0;
      line_upper_sum_(line, i) = k - 1;
      max_description_lens_[line] = std::max(max_description_lens_[line], k);

      for (int j = 1; j <= k; ++j) {
        line_sum_(line, i) += descriptions_(line, i, j);
        sum_line_sum_[line] += descriptions_(line, i, j);
      }
      line_upper_sum_(line, i) += line_sum_(line, i);
    }
  }
  std::cout << "I get a " << board_size_[Line::ROW] << " X "
            << board_size_[Line::COL] << " nonogram.\n";
}

void Solver::Print(bool show_descriptions) {
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
    total_row = board_size_[Line::ROW] + max_description_lens_[Line::COL];
    total_col = board_size_[Line::COL] + max_description_lens_[Line::ROW];
  } else {
    total_row = board_size_[Line::ROW] + 1;
    total_col = board_size_[Line::COL] + 1;
  }
  for (int row = 0; row < total_row; ++row) {
    for (int col = 0; col < total_col; ++col) {
      gram_for_printing_(row, col) = ' ';
    }
  }

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

  // output to terminal
  for (int i = 0; i < total_row; ++i) {
    for (int j = 0; j < total_col; ++j) {
      std::cout << std::setw(2) << gram_for_printing_(i, j);
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  // for (int i = 0; i <= board_size_[Line::ROW]; ++i) {
  //   for (int j = 0; j <= board_size_[Line::COL]; ++j) {
  //     std::cout << std::setw(2) << gram_(i, j) << ' ';
  //   }
  //   std::cout << '\n';
  // }

  // for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
  //   std::cout << (line == Line::ROW ? "Row:" : "Col:") << '\n';
  //   for (int i = 1; i <= board_size_[line]; ++i) {
  //     std::cout << "A ";
  //     for (int j = 0; j <= 2 * descriptions_(line, i, 0) + 1; ++j) {
  //       std::cout << A_(line, i, j) << ' ';
  //     }
  //     std::cout << '\n';
  //     std::cout << "B ";
  //     for (int j = 0; j <= 2 * descriptions_(line, i, 0) + 1; ++j) {
  //       std::cout << B_(line, i, j) << ' ';
  //     }
  //     std::cout << '\n';
  //   }
  //   std::cout << '\n';
  // }
}

template <typename T>
int Solver::ParseLine_(std::string l,
                       typename std::vector<T>::iterator begin,
                       typename std::vector<T>::iterator end,
                       size_t assign_from) {
  begin += assign_from - 1;
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

bool Solver::Initialize_() {
  for (int i = 0; i <= board_size_[Line::ROW]; ++i) {
    for (int j = 0; j <= board_size_[Line::COL]; ++j) {
      gram_(i, j) = Block::UNKNOWN;
    }
  }
  // initialize a_ and b_
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    for (int i = 1; i <= board_size_[line]; ++i) {
      offset_(line, i) = board_size_[1 - line] - line_upper_sum_(line, i);
      a_(line, i, 0) = 0;
      a_(line, i, 1) = 0;
      b_(line, i, 0) = 0;
      b_(line, i, 1) = offset_(line, i);
      for (int j = 2; j < 2 * descriptions_(line, i, 0) + 1; ++j) {
        a_(line, i, j) = j & 1 ? 1 : descriptions_(line, i, j / 2);
        b_(line, i, j) =
            j & 1 ? offset_(line, i) + 1 : descriptions_(line, i, j / 2);
      }
      a_(line, i, 2 * descriptions_(line, i, 0) + 1) = 0;
      b_(line, i, 2 * descriptions_(line, i, 0) + 1) = offset_(line, i);
    }
  }
  // compute A_ and B_
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    for (int i = 1; i <= board_size_[line]; ++i) {
      A_(line, i, 0) = 0;
      B_(line, i, 0) = 0;
      for (int j = 1; j <= 2 * descriptions_(line, i, 0) + 1; ++j) {
        A_(line, i, j) = A_(line, i, j - 1) + a_(line, i, j);
        B_(line, i, j) = B_(line, i, j - 1) + b_(line, i, j);
      }
    }
  }
  return sum_line_sum_[Line::ROW] == sum_line_sum_[Line::COL];
}

void Solver::FillSimpleBlocks_() {
  std::cout << "Pre-filling...\n";
  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    for (int i = 1; i <= board_size_[line]; ++i) {
      int start = offset_(line, i) + 1, end = 1;
      for (int j = 1; j <= descriptions_(line, i, 0); ++j) {
        end += descriptions_(line, i, j);
        if (descriptions_(line, i, j) > offset_(line, i)) {
          for (int k = start; k < end; ++k) {
            if (line == Line::ROW) {
              gram_(i, k) = Block::BLACK;
            } else if (line == Line::COL) {
              gram_(k, i) = Block::BLACK;
            }
          }
        }
        // start = end + offset + 1
        end += 1;
        start = end +
            offset_(line, i);  // or start += descriptions[line][i][j] + 1
      }
    }
  }

  // Marking blocks that are in 0-clue's line as blank
  for (int i = 1; i <= board_size_[Line::ROW]; ++i) {
    if (line_upper_sum_(Line::ROW, i) == 0) {
      for (int j = 1; j <= board_size_[Line::COL]; ++j) {
        gram_(i, j) = Block::BLANK;
      }
    }
  }
  for (int j = 1; j <= board_size_[Line::COL]; ++j) {
    if (line_upper_sum_(Line::COL, j) == 0) {
      for (int i = 1; i <= board_size_[Line::ROW]; ++i) {
        gram_(i, j) = Block::BLANK;
      }
    }
  }
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
  // for (int i = 0; i < unknown_pos_.size(); ++i) {
  //   std::cout << unknown_pos_[i].row << ' ' << unknown_pos_[i].col << '\n';
  // }
}

bool Solver::Check_(Solver::Pos pos) {
  int pp[2] = {pos.row, pos.col};

  for (int line = Line::ROW; line < Line::LOOP_END; ++line) {
    int l = board_size_[1 - line];
    int k = 2 * descriptions_(line, pp[line], 0) + 1;
    int h[Block::LOOP_END] = {0, 0};

    for (int i = 0; i <= l; ++i) {
      int block_type;
      if (line == Line::ROW) {
        block_type = gram_(pp[line], i);
      } else if (line == Line::COL) {
        block_type = gram_(i, pp[line]);
      }
      if (block_type != Block::UNKNOWN) {
        h[block_type] = i;
      }
      L_(Block::BLANK, i) = h[Block::BLACK];
      L_(Block::BLACK, i) = h[Block::BLANK];
    }
    for (int j = 0; j <= k; ++j) {
      for (int i = 0; i <= l; ++i) {
        fix_(j, i) = false;
      }
    }
    for (int i = 0; i <= k; ++i) {
      if (A_(line, pp[line], i) == 0) {
        fix_(i, 0) = true;
      }
    }

    int sigma = Block::BLANK;
    for (int j = 1; j <= k; ++j) {
      int i_from = std::max(1, A_(line, pp[line], j)),
          i_to = std::min(l, B_(line, pp[line], j));
      for (int i = i_from; i <= i_to; ++i) {
        int p_from =
            std::max(i - b_(line, pp[line], j),
                     std::max(A_(line, pp[line], j - 1), L_(sigma, i))),
            p_to = std::min(i - a_(line, pp[line], j),
                            B_(line, pp[line], j - 1));
        for (int p = p_from; p <= p_to; ++p) {
          if (fix_(j - 1, p)) {
            fix_(j, i) = true;
            break;
          }
        }
      }
      sigma = 1 - sigma;
    }

    if (!fix_(k, l)) return false;
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
