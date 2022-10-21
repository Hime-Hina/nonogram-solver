//
// Created by HimeHina on 2022/10/21.
//

#ifndef NONOGRAM_INCLUDES_TIMER_H_
#define NONOGRAM_INCLUDES_TIMER_H_

#include <chrono>
#include <iostream>

class Timer {
 public:
  Timer();

  ~Timer();

 private:
  std::chrono::high_resolution_clock::time_point _start, _end;
};

#endif //NONOGRAM_INCLUDES_TIMER_H_
