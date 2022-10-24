//
// Created by HimeHina on 2022/10/23.
//

#ifndef NONOGRAM_SRC_TEST_TESTER_HPP_
#define NONOGRAM_SRC_TEST_TESTER_HPP_

#include <iostream>
#include <functional>
#include <utility>
#include <vector>

class Tester {
 public:
  explicit Tester(std::string test_name = "") : test_name_(std::move(test_name)) {}

  Tester &AddTest(std::function<bool()> test_func) {
    test_funcs_.push_back(std::move(test_func));
    return *this;
  }

  int Run() {
    int i = 1;
    int counter_passed = 0;

    std::cout << test_name_ << ": Total " << test_funcs_.size() << " tests." << std::endl;
    for (auto &&f : test_funcs_) {
      std::cout << "Running test " << i << '/' << test_funcs_.size() << "... ";
      std::cout.flush();

      try {
        if (f()) {
          std::cout << "Passed." << std::endl;
          ++counter_passed;
        } else {
          std::cout << "Failed." << std::endl;
        }
      } catch (const std::exception &e) {
        std::cerr << "\nException: " << e.what() << std::endl;
        return counter_passed;
      } catch (const std::string &e) {
        std::cerr << "\nException: " << e << std::endl;
        return counter_passed;
      } catch (...) {
        std::exception_ptr ptr = std::current_exception();
        std::cerr << "\nException: "
                  << (ptr != nullptr ? ptr.__cxa_exception_type()->name() : "null")
                  << std::endl;
      }
      ++i;
    }
    std::cout << "Total " << counter_passed << '/' << test_funcs_.size() << " tests passed." << std::endl;

    return counter_passed;
  }

  std::size_t total_tests() const { return test_funcs_.size(); }

  ~Tester() = default;
 protected:
  std::string test_name_;
  std::vector<std::function<bool()>> test_funcs_;
};

#endif //NONOGRAM_SRC_TEST_TESTER_HPP_
