//
// Created by HimeHina on 2022/10/23.
//

#ifndef NONOGRAM_INCLUDES_SOLVER_HPP_
#define NONOGRAM_INCLUDES_SOLVER_HPP_

#include "Utilities/Timer.h"
#include "Utilities/Array2D.hpp"
#include "Utilities/Array3D.hpp"

class Solver {
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

  Solver(size_t rows, size_t cols)
      : total_blocks_(rows * cols), board_size_{rows, cols}, gram_(rows + 1, cols + 1),
        gram_for_printing_(rows + (cols + 1) / 2, cols + (rows + 1) / 2),
        max_description_lens_{0, 0},
        descriptions_(Line::LOOP_END,
                      std::max(rows, cols) + 1,
                      (std::max(rows, cols) + 1) / 2 + 1),
        line_sum_(Line::LOOP_END, std::max(rows, cols) + 1),
        line_upper_sum_(Line::LOOP_END, std::max(rows, cols) + 1),
        sum_line_sum_{0, 0},
        offset_(Line::LOOP_END, std::max(rows, cols) + 1),
        a_(Line::LOOP_END, std::max(rows, cols) + 1, std::max(rows, cols) + 3),
        b_(Line::LOOP_END, std::max(rows, cols) + 1, std::max(rows, cols) + 3),
        A_(Line::LOOP_END, std::max(rows, cols) + 1, std::max(rows, cols) + 3),
        B_(Line::LOOP_END, std::max(rows, cols) + 1, std::max(rows, cols) + 3),
        L_(Block::LOOP_END, std::max(rows, cols) + 1),
        fix_(std::max(rows, cols) + 3, std::max(rows, cols)) {}

  bool Solve();

  void Input();
  void Print(bool show_descriptions = true);

  ~Solver() = default;

 protected:
  constexpr static const char *_description_number_digits =
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  size_t total_blocks_;
  size_t board_size_[Line::LOOP_END];

//  int gram_(MAX_SIZE + 1, MAX_SIZE + 1);  // 1-indexed
  constexpr static const char block_chars_[Block::LOOP_END + 1] = {'.', '@', '?'};
  Array2D<int> gram_;
  Array2D<char> gram_for_printing_;

  // Array2D<int> _description_lens;  // 1-indexed
  // the descriptions of a nonogram always are this formal:
  // 0{0,offset}1{d_1}0{1,offset+1}1{d_2}0{1,offset+1}...0{1,offset+1}1{d_k}0{0,offset},
  // where offset = board_size_[line] - (k - 1 + \sum_{i=1}^{k} d_i)
  int max_description_lens_[Line::LOOP_END];
  Array3D<int> descriptions_;  // 1-indexed

  // for description d_1 d_2 ... d_k, its line upper sum is
  // k - 1 + linesum_i = k - 1 + \sum_{i=1}^{k} d_i
  Array2D<int> line_sum_;        // 1-indexed
  Array2D<int> line_upper_sum_;  // 1-indexed
  int sum_line_sum_[Line::LOOP_END];
  // offset_i is board_size - line_upper_sum_i,
  // if line_upper_sum_i is about row, board_size is the size of column;
  // otherwise is the size of row
  Array2D<int> offset_;  // 1-indexed

  // the length of the 3rd dimension can be determined with 2 * k + 1, where k =
  // floor((size + 1) / 2), so 2 * k + 1 <= size + 2.
  // then insert a A_0 to A_ or a B_0 to B_, finally, the length is size + 3
  Array3D<int> a_;
  Array3D<int> b_;
  // prefix sum of a_i
  Array3D<int> A_;
  // ............. b_i
  Array3D<int> B_;
  Array2D<int> L_;
  // if the definition of Fix is Fix(s^(i),d^(j)) instead Fix(d^(j),s^(i)),
  // we will get a lot of cache miss...
  Array2D<bool> fix_;

  Array<Pos> unknown_pos_;

  template <typename T>
  static int ParseLine_(std::string l,
                        typename std::vector<T>::iterator begin,
                        typename std::vector<T>::iterator end,
                        size_t assign_from = 0);

  bool Initialize_();

  void FillSimpleBlocks_();

  bool Check_(Pos pos);

  bool DFS_(int index);
};

#endif //NONOGRAM_INCLUDES_SOLVER_HPP_
