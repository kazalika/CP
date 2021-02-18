vector<int> z(string &s) {
  int n = s.size(), lst = 0, len;
  vector<int> z(n);
  for (int i = 1; i < n; ++i) {
    len = 0;
    if (lst + z[lst] >= i) len = min(z[i - lst], lst + z[lst] - i);
    while (i + len < n && s[i + len] == s[len]) ++len;
    z[i] = len;
    if (i + len > lst + z[lst]) lst = i;
  }
  z[0] = n;
  return z;
}
