/*******************************************

    Disjoint Set Union (DSU) by Kazalika
    O(alpha(n)) ( O(1) )
    path compression + union by rank

 *******************************************/

struct dsu {
  int size;
  vector<int> p, s;
  dsu() : size(0) {}
  dsu(int size_) : size(size_) {
    p.resize(size);
    s.resize(size, 1);
    iota(p.begin(), p.end(), 0);
  }
  int get_par(int x) {
    return (p[x] == x ? x : (p[x] = get_par(p[x])));
  }
  bool connect(int a, int b) {
    return get_par(a) == get_par(b);
  }
  void merge(int a, int b) {
    a = get_par(a), b = get_par(b);
    if (a == b)
      return;
    if (s[a] > s[b])
      swap(a, b);
    p[a] = b;
    s[b] += s[a];
  }
};
