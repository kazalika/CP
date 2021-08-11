/***********************************************

     Min cost max flow by Kazalika
     -O(n^2 m^2)
     -Bellman-Ford algorithm

************************************************/

const int inf = 1e9 + 9;
#define sz(x) (int)(x).size()
typedef long long ll;

struct edge {
  int fr, to; ll w, f, c; int link;
};
struct mcmf {
  int V, S{}, T{};
  vector<vector<int>> g;
  vector<edge> e;
  ll fl, co;

  mcmf() : V(0), fl(0), co(0) {}
  explicit mcmf(int V_) : V(V_ + 2), fl(0), co(0), S(V_), T(V_ + 1) {}

  void add_edge(int fr, int to, int w, int c) {
    if (fr == -1) fr = S;
    if (to == -1) to = T;
    e.push_back({fr, to, w, 0, c, sz(e) + 1}), e.push_back({to, fr, -w, 0, 0, sz(e) - 1});
    g[fr].push_back(sz(e) - 2), g[to].push_back(sz(e) - 1);
  }
  pair<ll, ll> get() {
    while (true) {
      vector<int> d(V, inf), p(V, -1);
      d[S] = 0;
      bool nd = 1;
      for (int t = 0; t < V && nd; ++t) {
        nd = 0;
        for (auto i = 0; i < sz(e); ++i) {
          auto ed = e[i];
          if (ed.f < ed.c && d[ed.fr] + ed.w < d[ed.to])
            d[ed.to] = d[ed.fr] + ed.w, p[ed.to] = i, nd = 1;
        }
      }
      if (d[T] == inf) break;
      ll af = inf;
      for (int v = T; v != S; v = e[p[v]].fr)
        af = min(af, e[p[v]].c - e[p[v]].f);
      for (int v = T; v != S; v = e[p[v]].fr)
        e[p[v]].f += af, e[e[p[v]].link].f -= af, co += e[p[v]].w * af;
      fl += af;
    }
    return {fl, co};
  }
};
