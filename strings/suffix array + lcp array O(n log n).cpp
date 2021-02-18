vector<int> c, p, cc, pp, cnt, pos;
int n;

void count_sort() {
  int m = max(n, 256);
  cnt.resize(m); pos.resize(m);
  fill(cnt.begin(), cnt.end(), 0);
  for (int x : c)
    cnt[x]++;
  pos[0] = 0;
  for (int i = 1; i < m; ++i)
    pos[i] = pos[i - 1] + cnt[i - 1];
  for (int i = 0; i < n; ++i) {
    int x = c[p[i]];
    pp[pos[x]++] = p[i];
  }
  pp.swap(p);
}

vector<int> suffix_array(string s) {
  s += '$';
  n = s.size();
  c.clear(); p.clear();
  cc.clear(); pp.clear();
  cnt.clear(); pos.clear();
  c.resize(n); p.resize(n);
  cc.resize(n); pp.resize(n);
  for (int i = 0; i < n; ++i) {
    c[i] = s[i];
    p[i] = i;
  }
  count_sort();

  int k = 0;
  while ((1 << k) < n) {
    for (int i = 0; i < n; ++i)
      p[i] = (p[i] - (1 << k) + n) % n;
    count_sort();
    cc[p[0]] = 0;
    for (int i = 1; i < n; ++i) {
      pair<int, int> now = { c[p[i]], c[(p[i] + (1 << k)) % n]};
      pair<int, int> prev = { c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]};
      cc[p[i]] = cc[p[i - 1]];
      if (now != prev)
        cc[p[i]]++;
    }
    cc.swap(c);
    k++;
  }

  vector<int> rt(n - 1);
  for (int i = 1; i < n; ++i)
    rt[i - 1] = p[i];
  return rt;
}

vector<int> lcp_array(string &s, vector<int> &suf) {
  n = s.size();
  vector<int> lcp(n), obr(n);
  for (int i = 0; i < n; ++i)
    obr[suf[i]] = i;
  int k = 0;
  for (int i = 0; i < n - 1; ++i) {
    int id = obr[i];
    if (!id) {
      k = 0;
      continue;
    }
    int j = suf[id - 1];
    if (k)
      k--;
    while (max(i + k, j + k) < n && s[i + k] == s[j + k])
      k++;
    lcp[id] = k;
  }
  return lcp;
}
