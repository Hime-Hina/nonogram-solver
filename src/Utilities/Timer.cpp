//
// Created by HimeHina on 2022/10/21.
//

#include "Utilities/Timer.h"
Timer::Timer() {
  using namespace std::chrono;
  _start = high_resolution_clock::now();
}
Timer::~Timer() {
  using namespace std::chrono;
  _end = high_resolution_clock::now();
  auto time_span = duration_cast<microseconds>(_end - _start);
  std::cout << "Timer: " << time_span.count() << " um.\n";
}
