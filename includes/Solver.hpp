//
// Created by HimeHina on 2022/10/23.
//

#ifndef NONOGRAM_INCLUDES_SOLVER_HPP_
#define NONOGRAM_INCLUDES_SOLVER_HPP_

#include "Utilities/Timer.h"
#include "Utilities/Array2D.hpp"
#include "Utilities/Array3D.hpp"

struct Line {
  enum { ROW = 0, COL, LOOP_END };
};
struct Block {
  enum { BLANK = 0, BLACK, LOOP_END, UNKNOWN = 2 };
};
struct Pos {
  int row, col;
};

class Solver {
 public:
  struct IncorrectDescriptions : public std::exception {
    const char *what() const noexcept override {
      return "The descriptions is incorrect!";
    }
  };
  struct EmptyDescriptions : public std::exception {
    const char *what() const noexcept override {
      return "Can not solve an empty nonogram!";
    }
  };

  Solver() = default;
  Solver(std::size_t rows, std::size_t cols)
      : total_blocks_(rows * cols),
        board_size_{rows, cols},
        gram_(rows + 1, cols + 1, Block::UNKNOWN),
        descriptions_(
            Line::LOOP_END,
            std::max(rows, cols) + 1,
            (std::max(rows, cols) + 1) / 2 + 1
        ),
        line_sum_(Line::LOOP_END, std::max(rows, cols) + 1),
        line_upper_sum_(Line::LOOP_END, std::max(rows, cols) + 1),
        offset_(Line::LOOP_END, std::max(rows, cols) + 1),
        a_(Line::LOOP_END, std::max(rows, cols) + 1, std::max(rows, cols) + 3),
        b_(Line::LOOP_END, std::max(rows, cols) + 1, std::max(rows, cols) + 3),
        A_(Line::LOOP_END, std::max(rows, cols) + 1, std::max(rows, cols) + 3),
        B_(Line::LOOP_END, std::max(rows, cols) + 1, std::max(rows, cols) + 3) {
    for (std::size_t i = 0; i < gram_.rows(); ++i) {
      gram_(i, 0) = Block::BLANK;
    }
  }

  void Init(std::size_t rows, std::size_t cols);

  bool Solve();

  void Print(bool show_descriptions = true, std::ostream &out = std::cout);

  friend std::istream &operator>>(std::istream &in, Solver &solver);
  friend std::ostream &operator<<(std::ostream &out, Solver &solver);

  ~Solver() = default;

 protected:
  constexpr static const char *_description_number_digits =
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  std::size_t total_blocks_ = 0;
  std::size_t board_size_[Line::LOOP_END]{0, 0};

  constexpr static const char block_chars_[Block::LOOP_END + 1] = {'.', '@', '?'};
  Array2D<int> gram_; // 1-indexed
  Array2D<char> gram_for_printing_;

  // Array2D<int> _description_lens;  // 1-indexed
  // the descriptions of a nonogram always are this formal:
  // 0{0,offset}1{d_1}0{1,offset+1}1{d_2}0{1,offset+1}...0{1,offset+1}1{d_k}0{0,offset},
  // where offset = board_size_[line] - (k - 1 + \sum_{i=1}^{k} d_i)
  int max_description_lens_[Line::LOOP_END]{0, 0};
  Array3D<int> descriptions_;  // 1-indexed

  // for description d_1 d_2 ... d_k, its line upper sum is
  // k - 1 + line_sum_i = k - 1 + \sum_{i=1}^{k} d_i
  int sum_line_sum_[Line::LOOP_END]{0, 0};
  Array2D<int> line_sum_;        // 1-indexed
  Array2D<int> line_upper_sum_;  // 1-indexed
  // offset_i is board_size - line_upper_sum_i,
  // if line_upper_sum_i is about row, board_size is the size of column;
  // otherwise is the size of row
  Array2D<int> offset_;  // 1-indexed

  // The array listed below are all 1-indexed.
  // the length of the 3rd dimension can be determined with 2 * k + 1, where k =
  // floor((size + 1) / 2), so 2 * k + 1 <= size + 2.
  // then insert a A_0 to A_ or a B_0 to B_, finally, the length is size + 3
  Array3D<int> a_;
  Array3D<int> b_;
  Array3D<int> A_; // prefix sum of a_i
  Array3D<int> B_; // ............. b_i

  // used for dfs
  Array<Pos> unknown_pos_;

  template <typename T, std::enable_if_t<is_iterator_v<T>, bool> = true>
  static int ParseLine_(std::string l, T begin, T end, std::size_t assign_from = 0);

  void InitializeForPrinting_(bool show_descriptions = true);

  void Initialize_();

  void FillSimpleBlocks_();

  void InitializeAt_(std::size_t line, std::size_t index);

  bool IsFixable_(Array2D<bool> &is_fix, std::size_t line, std::size_t index);

  Array<int> Settle_(std::size_t line, std::size_t index);

  bool Check_(Pos pos);

  bool Guess_(int index);

};

#endif //NONOGRAM_INCLUDES_SOLVER_HPP_
