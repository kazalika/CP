#include <bits/stdc++.h>
using namespace std;

/**************************************************
      Segment Tree on minimum by Kazalika
                 O(log) query
**************************************************/

template <typename T>
struct segment_tree {
  int size;
  T inf;
  vector<T> t;
  segment_tree(vector<T> &v) {
    inf = INT_MAX;
    if (inf + 1 > 0)
      inf = LLONG_MAX;
    size = 1;
    while (size < v.size())
      size <<= 1;
    t.resize(size << 1);
    for (int i = 0; i < v.size(); ++i)
      t[size + i] = v[i];
    for (int i = (int) v.size() - 1; i > 0; --i)
      t[i] = min(t[i << 1], t[t << 1 | 1]);
  }
  void upd(int i, T vl) {
    i += size;
    t[i] = vl;
    while (i >>= 1)
      t[i] = min(t[i << 1], t[t << 1 | 1]);
  }
  T get(int l, int r) {
    T ret = inf;
    for (int f = l + size, s = r + size; f < s; f >>= 1, s >>= 1) {
      if (f & 1)
        ret = min(ret, t[f]), f++;
      if (s & 1)
        s--, ret = min(ret, t[s]);
    }
    return ret;
  }
};
