#include <io.h>

#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

int GetFileNum(std::string path) {
  path += "\\*.*";
  struct _finddata_t fileinfo;
  intptr_t handle = _findfirst(path.c_str(), &fileinfo);
  int file_num = 0;

  if (handle == -1) return -1;
  while (!_findnext(handle, &fileinfo)) {
    if (0 == strcmp(fileinfo.name, "..") || _A_SUBDIR == fileinfo.attrib) {
      continue;
    }
    ++file_num;
  }
  _findclose(handle);

  return file_num;
}

int main() {
  using namespace std;

  const string out_path = ".\\samples";
  const char *digits =
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*("
      ")_+";

  uniform_int_distribution<> idist(16, 64);
  uniform_int_distribution<> bdist(0, 1);
  default_random_engine eng(
      chrono::high_resolution_clock::now().time_since_epoch().count());
  auto rnd_size = bind(idist, eng), rnd_block = bind(bdist, eng);

  int files_num = GetFileNum(out_path);
  ofstream fs;
  fs.open(out_path + "\\" + to_string(files_num + 1) + ".in",
          ios::out | ios::trunc);

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

  fs << n << ' ' << m << '\n';
  cout << n << ' ' << m << '\n';

  int sum[2] = {0, 0};
  for (int i = 0; i < n; ++i) {
    int k = 0;
    int consecutive = 0;
    for (int j = 0; j < m; ++j) {
      if (gram[i][j] == 1) {
        ++consecutive;
        if (j == m - 1 || gram[i][j + 1] == 0) {
          fs << consecutive << ' ';
          cout << consecutive << ' ';
          sum[0] += consecutive;
          ++k;
        }
      } else {
        consecutive = 0;
      }
    }
    if (k = 0) cout << '0';
    fs << '\n';
    cout << '\n';
  }

  fs << '\n';
  cout << '\n';

  for (int j = 0; j < m; ++j) {
    int k = 0;
    int consecutive = 0;
    for (int i = 0; i < n; ++i) {
      if (gram[i][j] == 1) {
        ++consecutive;
        if (i == n - 1 || gram[i + 1][j] == 0) {
          fs << consecutive << ' ';
          cout << consecutive << ' ';
          sum[1] += consecutive;
          ++k;
        }
      } else {
        consecutive = 0;
      }
    }
    if (k == 0) cout << '0';
    fs << '\n';
    cout << '\n';
  }

  fs << '\n';

  fs << setw(2) << ' ';
  for (int j = 1; j <= m; ++j) {
    fs << setw(2) << digits[j];
  }
  fs << '\n';
  for (int i = 0; i < n; ++i) {
    fs << setw(2) << digits[i + 1];
    for (int j = 0; j < m; ++j) {
      if (gram[i][j]) {
        fs << setw(2) << '@';
      } else {
        fs << setw(2) << '.';
      }
    }
    fs << '\n';
  }

  fs.close();

  return 0;
}