pair<vector<int>, vector<int>> manaker(string &s) {  // { odd, even }
  int n = s.size(), lst = 0, len;
  vector<int> o(n), e(n);
  o[0] = 1;
  for (int i = 1; i < n; ++i) {
    len = 1;
    if (lst + o[lst] > i) len = min(lst + o[lst] - i, o[lst - (i - lst)]);
    while (i + len < n && i - len >= 0 && s[i + len] == s[i - len]) len++;
    o[i] = len;
    if (i + len > lst + o[lst]) lst = i;
  }
  lst = 0;
  for (int i = 0; i < n - 1; ++i) { // |--^---|
    len = 0;
    if (lst + e[lst] > i) len = min(lst + e[lst] - i, e[lst - (i - lst)]);
    len = max(0, len);
    while (i + len + 1 < n && i - len >= 0 && s[i + len + 1] == s[i - len]) len++;
    e[i] = len;
    if (i + len > lst + e[lst]) lst = i;
  }
  return {o, e};
}
