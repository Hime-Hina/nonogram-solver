#include <io.h>

#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <filesystem>

int GetFileNum(std::string path) {
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

int main() {
  using namespace std;

  const string out_path = "..\\samples";
  const char *digits =
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*("
      ")_+";

  uniform_int_distribution<> idist(16, 64);
  uniform_int_distribution<> bdist(0, 1);
  default_random_engine eng(
      chrono::high_resolution_clock::now().time_since_epoch().count());
  auto rnd_size = bind(idist, eng), rnd_block = bind(bdist, eng);

  int files_num = GetFileNum(out_path);
  ofstream ofs;
  ofs.open(
      out_path + "\\" + to_string(files_num + 1) + ".in",
      ios::out | ios::trunc
  );

  int n = rnd_size(), m = rnd_size();
  vector<vector<bool>> gram(n);
  for (int i = 0; i < n; ++i) {
    gram[i].resize(m);
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      gram[i][j] = rnd_block();
    }
  }

  ofs << n << ' ' << m << '\n';
  cout << n << ' ' << m << '\n';

  int sum[2] = {0, 0};
  for (int i = 0; i < n; ++i) {
    int k = 0;
    int consecutive = 0;
    for (int j = 0; j < m; ++j) {
      if (gram[i][j] == 1) {
        ++consecutive;
        if (j == m - 1 || gram[i][j + 1] == 0) {
          ofs << consecutive << ' ';
          cout << consecutive << ' ';
          sum[0] += consecutive;
          ++k;
        }
      } else {
        consecutive = 0;
      }
    }
    if (k = 0) cout << '0';
    ofs << '\n';
    cout << '\n';
  }

  ofs << '\n';
  cout << '\n';

  for (int j = 0; j < m; ++j) {
    int k = 0;
    int consecutive = 0;
    for (int i = 0; i < n; ++i) {
      if (gram[i][j] == 1) {
        ++consecutive;
        if (i == n - 1 || gram[i + 1][j] == 0) {
          ofs << consecutive << ' ';
          cout << consecutive << ' ';
          sum[1] += consecutive;
          ++k;
        }
      } else {
        consecutive = 0;
      }
    }
    if (k == 0) cout << '0';
    ofs << '\n';
    cout << '\n';
  }

  ofs << '\n';

  ofs << setw(2) << ' ';
  for (int j = 1; j <= m; ++j) {
    ofs << setw(2) << digits[j];
  }
  ofs << '\n';
  for (int i = 0; i < n; ++i) {
    ofs << setw(2) << digits[i + 1];
    for (int j = 0; j < m; ++j) {
      if (gram[i][j]) {
        ofs << setw(2) << '@';
      } else {
        ofs << setw(2) << '.';
      }
    }
    ofs << '\n';
  }

  ofs.close();

  return 0;
}