#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll mod = 1e9 + 7;

struct segment_tree {
  int size;
  vector<ll> sum, add;

  void build(int t, int l, int r, vector<ll> &vals, vector<int> &fromTin) {
    if (l + 1 == r) {
      sum[t] = vals[fromTin[l]];
      return;
    }
    int md = r + l >> 1;
    build(t * 2 + 1, l, md, vals, fromTin);
    build(t * 2 + 2, md, r, vals, fromTin);
    sum[t] = sum[t * 2 + 1] + sum[t * 2 + 2];
  }
  void push(int t, int l, int r) {
    if (!add[t])
      return;
    sum[t] += add[t] * (r - l);
    if (l + 1 != r) {
      add[t * 2 + 1] += add[t];
      add[t * 2 + 2] += add[t];
    }
    add[t] = 0;
  }

  segment_tree() {}
  segment_tree(vector<ll> &vals, vector<int> &fromTin) {
    size = vals.size();
    sum.resize(4 * size);
    add.resize(4 * size);
    build(0, 0, size, vals, fromTin);
  }

  ll get_module(int t, int l, int r, int x, int y) {
    push(t, l, r);
    if (l >= x && r <= y)
      return sum[t] % mod;
    if (l >= y || r <= x)
      return 0;
    int md = r + l >> 1;
    return (get_module(t * 2 + 1, l, md, x, y) + get_module(t * 2 + 2, md, r, x, y)) % mod;
  }
  ll get_modulo(int l, int r) { return get_module(0, 0, size, l, r + 1); }
  ll get(int t, int l, int r, int x, int y) {
    push(t, l, r);
    if (l >= x && r <= y)
      return sum[t];
    if (l >= y || r <= x)
      return 0;
    int md = r + l >> 1;
    return get(t * 2 + 1, l, md, x, y) + get(t * 2 + 2, md, r, x, y);
  }
  ll get(int l, int r) { return get(0, 0, size, l, r + 1); }

  void upd(int t, int l, int r, int x, int y, ll val) {
    push(t, l, r);
    if (l >= x && r <= y) {
      add[t] = val;
      push(t, l, r);
      return;
    }
    if (l >= y || r <= x)
      return;
    int md = r + l >> 1;
    upd(t * 2 + 1, l, md, x, y, val);
    upd(t * 2 + 2, md, r, x, y, val);
    sum[t] = sum[t * 2 + 1] + sum[t * 2 + 2];
  }
  void upd(int l, int r, ll val) { upd(0, 0, size, l, r + 1, val); }
};

struct heavy_light_decomposition {
  int size, timer;
  vector<int> tin, tout, p, h, fromTin, szs;
  vector<vector<int>> g;
  vector<ll> vals;
  segment_tree sgt;

  int init_sizes(int v, int par = -1) {  // sizes, p, szs, rebuild
    p[v] = par;
    szs[v] = 1;
    int b = g[v].size();
    if (par != -1)
      b--;
    for (int i = 0; i < b; ++i) {
      if (g[v][i] == par)
        swap(g[v][i], g[v].back());
      szs[v] += init_sizes(g[v][i], v);
      if (szs[g[v][i]] > szs[g[v][0]])
        swap(g[v][i], g[v][0]);
    }
    if (par != -1)
      g[v].pop_back();
    return szs[v];
  }

  void init_hld(int v) {  // tin, tout, h, fromTin
    tin[v] = timer++;
    fromTin[tin[v]] = v;
    if (g[v].empty()) {
      tout[v] = timer;
      return;
    }
    h[g[v][0]] = h[v];
    init_hld(g[v][0]);
    for (int i = 1; i < g[v].size(); ++i) {
      h[g[v][i]] = g[v][i];
      init_hld(g[v][i]);
    }
    tout[v] = timer;
  }

  heavy_light_decomposition() {}
  heavy_light_decomposition(vector<vector<int>> &graph, vector<ll> &values) {
    timer = 0;
    size = graph.size();
    g = graph;
    vals = values;
    szs = tin = tout = p = h = fromTin = vector<int>(size);
    init_sizes(0);
    init_hld(0);
    sgt = segment_tree(vals, fromTin);
  }

  bool upper(int a, int b) { return tin[a] <= tin[b] && tout[a] >= tout[b]; }

  ll get_vertex(int a) {
    return sgt.get(tin[a], tin[a]);
  }

  void ug_modulo(int &a, int b, ll &ans) {
    while (!upper(h[a], b)) {
      ans = (ans + sgt.get_modulo(tin[h[a]], tin[a])) % mod;
      a = p[h[a]];
    }
  }

  ll getWay_modulo(int a, int b) {
    if (min(a, b) < 0)
      return 0;
    ll rt = 0;
    ug_modulo(a, b, rt);
    ug_modulo(b, a, rt);
    if (!upper(a, b))
      swap(a, b);
    rt = (rt + sgt.get_modulo(tin[a], tin[b])) % mod;
    return rt;
  }
  ll getSubtree_modulo(int a) {
    return sgt.get_modulo(tin[a], tout[a] - 1);
  }

  void ua(int &a, int b, ll val) {
    while (!upper(h[a], b)) {
      sgt.upd(tin[h[a]], tin[a], val);
      a = p[h[a]];
    }
  }

  void updWay(int a, int b, ll val) {
    if (min(a, b) < 0)
      return;
    ua(a, b, val);
    ua(b, a, val);
    if (!upper(a, b))
      swap(a, b);
    sgt.upd(tin[a], tin[b], val);
  }
  void updSubtree(int a, ll val) {
    sgt.upd(tin[a], tout[a] - 1, val);
  }
};
