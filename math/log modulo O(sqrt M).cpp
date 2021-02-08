template <typename T>
T log_mod(T a, T b, T m) {     // a ^ x = b (mod m) O(sqrt(m))
  T n = (T) sqrtl(m + .0) + 1; // -> a ^ (Q * n - q) = b (mod m)
  T an = 1;                    // -> a ^ Q * n = b ^ q (mod m)
  for (T i = 0; i < n; ++i)
    an = (an * a) % m;
  map<T, T> vals;
  for (T i = 1, cur = an; i <= n; ++i) {
    if (!vals.count(cur))
      vals[cur] = i;
    cur = (cur * an) % m;
  }
  for (T i = 0, cur = b; i <= n; ++i) {
    if (vals.count(cur)) {
      T ans = vals[cur] * n - i; // Q * n - q
      if (ans < m) return ans;
    }
    cur = (cur * a) % m;
  }
  return -1; // or -inf
}
