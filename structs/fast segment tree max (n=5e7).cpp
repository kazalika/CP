#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

/**************************************************
      Segment Tree on maximum by Kazalika
                 O(log) query
             1.5-1.7s (n = 2e7, int)
             9.2-9.6s (n = 5e7, int)
**************************************************/

const int SZ = 5e7;
int t[SZ << 1];

void upd(int i, int vl) {
  i += SZ - 1;
  t[i] = vl;
  while (i >>= 1)
    t[i] = max(t[i << 1], t[i << 1 | 1]);
}

int get(int l, int r) {
  l--;
  int ret = 0;
  for (int f = l + SZ, s = r + SZ; f < s; f >>= 1, s >>= 1) {
    if (f & 1)
      ret = max(ret, t[f]), f++;
    if (s & 1)
      s--, ret = max(ret, t[s]);
  }
  return ret;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  ll c, d, x, ans = 0, res = 0, l, r;
  cin >> t[0 + SZ] >> c >> d >> x;

  for (int i = 1; i < n; ++i) {
    t[i + SZ] = (((ll)t[i - 1 + SZ] * c + d) & ((1LL << 30) - 1));
  }
  for (int i = SZ - 1; i > 0; --i) {
    t[i] = max(t[i << 1], t[i << 1 | 1]);
  }
  
  for (int i = 0; i < n; ++i) {
    l = (x + ans) % n;
    r = (l + i) % n;
    x = ((x * 1103515245 + 12345) & ((1LL << 30) - 1));
    ans = get(min(l, r) + 1, max(l, r) + 1);
    res += ans;
  }
  cout << res;
}
