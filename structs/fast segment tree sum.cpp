/**************************************************
          Segment Tree on sum by Kazalika
                 O(log) query
**************************************************/

template <typename T>
struct segment_tree {
  int size;
  vector<T> t;
  segment_tree(vector<T> &v) {
    size = 1;
    while (size < v.size())
      size <<= 1;
    t.resize(size << 1);
    for (int i = 0; i < v.size(); ++i)
      t[size + i] = v[i];
    for (int i = size - 1; i > 0; --i)
      t[i] = t[i << 1] + t[i << 1 | 1];
  }
  void upd(int i, T vl) {
    i += size - 1;
    t[i] = vl;
    while (i >>= 1)
      t[i] = t[i << 1] + t[i << 1 | 1];
  }
  T get(int l, int r) {
    l--;
    T ret = 0;
    for (int f = l + size, s = r + size; f < s; f >>= 1, s >>= 1) {
      if (f & 1)
        ret += t[f], f++;
      if (s & 1)
        s--, ret += t[s];
    }
    return ret;
  }
};
