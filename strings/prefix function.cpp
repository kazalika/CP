vector<int> prefix(string &s) {
  int n = s.size(), len;
  vector<int> p(n);
  for (int i = 1; i < n; ++i) {
    len = p[i - 1];
    while (len && s[len] != s[i]) len = p[len - 1];
    p[i] = len + int(s[len] == s[i]);
  }
  return p;
}
