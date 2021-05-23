#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

/**************************************************
      Segment Tree on maximum by Kazalika
                 O(log) query
             3.684s (n = 2e7, int)
**************************************************/

template <typename T>
struct segment_tree {
  int size;
  T inf;
  vector<T> t;
  segment_tree(vector<T> &v) {
    inf = INT_MAX + 1;
    if (inf > 0)
      inf = LLONG_MAX;
    else
      inf--;
    size = 1;
    while (size < v.size())
      size <<= 1;
    t.resize(size << 1, -inf);
    for (int i = 0; i < v.size(); ++i)
      t[size + i] = v[i];
    for (int i = size - 1; i > 0; --i)
      t[i] = max(t[i << 1], t[i << 1 | 1]);
  }
  void upd(int i, T vl) {
    i += size - 1;
    t[i] = vl;
    while (i >>= 1)
      t[i] = max(t[i << 1], t[t << 1 | 1]);
  }
  T get(int l, int r) {
    l--;
    T ret = -inf;
    for (int f = l + size, s = r + size; f < s; f >>= 1, s >>= 1) {
      if (f & 1)
        ret = max(ret, t[f]), f++;
      if (s & 1)
        s--, ret = max(ret, t[s]);
    }
    return ret;
  }
};


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int> a(n);
  ll b, c, d, x, ans = 0, res = 0;
  cin >> a[0] >> b >> c >> d >> x;
  for (int i = 1; i < n; ++i) {
    b = (b * c + d) % (1LL << 31);
    a[i] = (a[i - 1] + 1 - 2 * ((b % 239179) & 1));
  }
  segment_tree<int> sgt(a);

  for (int i = 0; i < n; ++i) {
    ll l = (x + ans % n + n) % n;
    ll r = (l + i) % n;
    x = (x * 1103515245 + 12345) % (1LL << 31);
    ans = sgt.get(min(l, r) + 1, max(l, r) + 1);
    res += ans;
  }
  cout << res;
}
