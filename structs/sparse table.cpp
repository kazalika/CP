#include <bits/stdc++.h>
using namespace std;

/**************************************************
      Sparse Table on minimum by Kazalika
                 O(log) query
**************************************************/

template <typename T>
struct sparse_table {
  int size;
  vector<vector<T>> sp;
  vector<int> lg;

  sparse_table() {}
  sparse_table(vector<T> &v) {
    size = v.size();
    lg.resize(size + 1);
    for (int i = 2; i <= size; ++i)
      lg[i] = lg[i >> 1] + 1;
    sp.resize(lg[size] + 1, vector<T>(size));
    for (int i = 0; i < size; ++i)
      sp[0][i] = v[i];
    for (int j = 1; j <= lg[size]; ++j)
      for (int i = 0; i < size; ++i)
        sp[j][i] = min(sp[j - 1][i], sp[j - 1][i + (1 << (j - 1))]);
  }
  T get_min(int l, int r) {
    int i = lg[r - l + 1];
    return min(sp[i][l], sp[i][r - (1 << i) + 1]);
  }
};
