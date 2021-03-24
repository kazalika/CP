#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;
typedef vector<int> vint;

struct segment_tree {
  int size;
  vector<ll> sum, add;

  ll bld(int t, int l, int r, vll &vs, vint &fT) {
    if (l + 1 == r) return sum[t] = vs[fT[l]];
    sum[t] = bld(t * 2 + 1, l, r + l >> 1, vs, fT) + bld(t * 2 + 2, r + l >> 1, r, vs, fT);
  }
  void push(int t, int l, int r) {
    if (!add[t]) return;
    sum[t] += add[t] * (r - l);
    if (l + 1 != r) add[t * 2 + 1] += add[t], add[t * 2 + 2] += add[t];
    add[t] = 0;
  }
  segment_tree() : size(0) {}
  segment_tree(vll &vs, vint &fT) : size(vs.size()), sum(vll(4 * size)), add(vll(4 * size)) { bld(0, 0, size, vs, fT); }

  ll get(int t, int l, int r, int x, int y) {
    push(t, l, r);
    if (l >= x && r <= y) return sum[t];
    if (l >= y || r <= x) return 0;
    return get(t * 2 + 1, l, r + l >> 1, x, y) + get(t * 2 + 2, r + l >> 1, r, x, y);
  }
  ll get(int l, int r) { return get(0, 0, size, l, r + 1); }

  ll upd(int t, int l, int r, int x, int y, ll vl) {
    push(t, l, r);
    if (l >= x && r <= y) {
      add[t] += vl;
      push(t, l, r);
      return sum[t];
    }
    if (l >= y || r <= x) return 0;
    sum[t] = upd(t * 2 + 1, l, r + l >> 1, x, y, vl) + upd(t * 2 + 2, r + l >> 1, r, x, y, vl);
  }
  void upd(int l, int r, ll vl) { upd(0, 0, size, l, r + 1, vl); }
};

struct HLD {
  int size, tmr;
  vint tin, tout, p, h, fT, szs;
  vector<vint> g;
  vll vs;
  segment_tree sgt;

  int init_szs(int v, int pr = -1) { // init : p, szs
    p[v] = pr, szs[v] = 1;
    int b = (int)g[v].size() - (pr != -1 ? 1 : 0);
    for (int i = 0; i < b; ++i) {
      if (g[v][i] == pr) swap(g[v][i], g[v].back());
      szs[v] += init_szs(g[v][i], v);
      if (szs[g[v][i]] > szs[g[v][0]]) swap(g[v][i], g[v][0]);
    }
    if (pr != -1) g[v].pop_back();
    return szs[v];
  }
  void init_hld(int v) {  // init : tin, fT, tout, h
    tin[v] = tmr++, fT[tin[v]] = v;
    if (!g[v].empty()) {
      h[g[v][0]] = h[v];
      init_hld(g[v][0]);
      for (int i = 1; i < g[v].size(); ++i) {
        h[g[v][i]] = g[v][i];
        init_hld(g[v][i]);
      }
    }
    tout[v] = tmr;
  }

  HLD() {}
  HLD(vector<vint> &gi, vll &vsi) : tmr(0), size(gi.size()), g(gi), vs(vsi) {
    szs = tin = tout = p = h = fT = vint(size);
    init_szs(0);
    init_hld(0);
    sgt = segment_tree(vs, fT);
  }

  bool upper(int a, int b) { return tin[a] <= tin[b] && tout[a] >= tout[b]; }
  ll getV(int a) { return sgt.get(tin[a], tin[a]); } // get one vertex "a"
  // get sum on vertical way, way, subtree
  void ug(int &a, int b, ll &ans) {
    while (!upper(h[a], b)) {
      ans += sgt.get(tin[h[a]], tin[a]);
      a = p[h[a]];
    }
  }
  ll get_way(int a, int b) {
    if (min(a, b) < 0) return 0;
    ll rt = 0;
    ug(a, b, rt), ug(b, a, rt);
    return rt + sgt.get(min(tin[a], tin[b]), max(tin[a], tin[b]));
  }
  ll get_subt(int a) {
    return sgt.get(tin[a], tout[a] - 1);
  }
  // add value on vertical way, way, subtree
  void ua(int &a, int b, ll vl) {
    while (!upper(h[a], b)) {
      sgt.upd(tin[h[a]], tin[a], vl);
      a = p[h[a]];
    }
  }
  void upd_way(int a, int b, ll vl) {
    if (min(a, b) < 0) return;
    ua(a, b, vl);
    ua(b, a, vl);
    sgt.upd(min(tin[a], tin[b]), max(tin[a], tin[b]), vl);
  }
  void upd_subt(int a, ll vl) {
    sgt.upd(tin[a], tout[a] - 1, vl);
  }
};
