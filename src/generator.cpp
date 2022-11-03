#include <io.h>

#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

class WhiteHole {
 public:
  static WhiteHole out;

  explicit WhiteHole(const std::string &out_path = "..\\samples")
      : files_num_(GetFileNum(out_path)),
        out_path_(out_path),
        ofs(
            out_path + "\\" + std::to_string(files_num_ + 1) + ".in",
            std::ios::out | std::ios::trunc
        ) {}

  void EnableLogger(bool is_logger = true) {
    is_logger_ = is_logger;
  }

  template <class T>
  WhiteHole &operator<<(const T &val) {
    if (is_logger_) std::cout << val;
    ofs << val;
    return *this;
  }

  ~WhiteHole() {
    ofs.close();
  }

 private:
  bool is_logger_ = true;
  int files_num_;
  std::string out_path_;
  std::ofstream ofs;

  static int GetFileNum(std::string path) {
    path += "\\*.*";
    struct _finddata_t fileinfo{};
    intptr_t handle = _findfirst(path.c_str(), &fileinfo);
    int file_num = 0;

    if (handle == -1) return -1;
    while (!_findnext(handle, &fileinfo)) {
      std::string file_name(fileinfo.name);
      if (".." == file_name || _A_SUBDIR == fileinfo.attrib) {
        continue;
      }
      if (file_name.find(".in") != std::string::npos) ++file_num;
    }
    _findclose(handle);

    return file_num;
  }

};
WhiteHole WhiteHole::out;

int main() {
  using namespace std;

  const char *digits =
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*("
      ")_+";

  uniform_int_distribution<> idist(16, 64);
  uniform_int_distribution<> bdist(0, 1);
  default_random_engine eng(
      chrono::high_resolution_clock::now().time_since_epoch().count());
  auto rnd_size = [&idist, &eng] { return idist(eng); };
  auto rnd_block = [&bdist, &eng] { return bdist(eng); };

  int n = 4/*rnd_size()*/, m = 4/*rnd_size()*/;
  vector<vector<bool>> gram(n);
  for (int i = 0; i < n; ++i) {
    gram[i].resize(m);
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      gram[i][j] = rnd_block();
    }
  }

  WhiteHole::out << n << ' ' << m << '\n';

  int sum[2] = {0, 0};
  for (int i = 0; i < n; ++i) {
    int k = 0;
    int consecutive = 0;
    for (int j = 0; j < m; ++j) {
      if (gram[i][j] == 1) {
        ++consecutive;
        if (j == m - 1 || gram[i][j + 1] == 0) {
          WhiteHole::out << consecutive << ' ';
          sum[0] += consecutive;
          ++k;
        }
      } else {
        consecutive = 0;
      }
    }
    if (k == 0) WhiteHole::out << '0';
    WhiteHole::out << '\n';
  }

  WhiteHole::out << '\n';

  for (int j = 0; j < m; ++j) {
    int k = 0;
    int consecutive = 0;
    for (int i = 0; i < n; ++i) {
      if (gram[i][j] == 1) {
        ++consecutive;
        if (i == n - 1 || gram[i + 1][j] == 0) {
          WhiteHole::out << consecutive << ' ';
          sum[1] += consecutive;
          ++k;
        }
      } else {
        consecutive = 0;
      }
    }
    if (k == 0) WhiteHole::out << '0';
    WhiteHole::out << '\n';
  }

  WhiteHole::out.EnableLogger(false);
  WhiteHole::out << '\n';

  WhiteHole::out << setw(2) << ' ';
  for (int j = 1; j <= m; ++j) {
    WhiteHole::out << setw(2) << digits[j];
  }
  WhiteHole::out << '\n';
  for (int i = 0; i < n; ++i) {
    WhiteHole::out << setw(2) << digits[i + 1];
    for (int j = 0; j < m; ++j) {
      if (gram[i][j]) {
        WhiteHole::out << setw(2) << '@';
      } else {
        WhiteHole::out << setw(2) << '.';
      }
    }
    WhiteHole::out << '\n';
  }
  WhiteHole::out.EnableLogger(true);

  return 0;
}