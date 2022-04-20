/***********************************************

     Min cost max flow by Kazalika
     -O(n^2 m^2)
     -Bellman-Ford algorithm

************************************************/

using ll = long long;

const ll inf = 1e12 + 9;
#define sz(x) (int)(x).size()
typedef long long ll;

struct edge {
  int fr, to; ll w, f, c; int link, id;
};

struct mcmf {
  int V, S, T;
  vector<vector<int>> g;
  vector<edge> e;
  ll fl, co;

  mcmf() : V(0), fl(0), co(0) {}
  explicit mcmf(int V_) : V(V_ + 2), fl(0), co(0), S(V_), T(V_ + 1), g(vector<vector<int>>(V_ + 2)) {}
  mcmf(int V_, int S_, int T_) : V(V_), fl(0), co(0), S(S_), T(T_), g(vector<vector<int>>(V_)) {}


  void add_edge(int fr, int to, ll w, ll c, int id) {
    if (fr == -1) fr = S;
    if (to == -1) to = T;
    e.push_back({fr, to, w, 0, c, sz(e) + 1, id}), e.push_back({to, fr, -w, 0, 0, sz(e) - 1, id});
    g[fr].push_back(sz(e) - 2), g[to].push_back(sz(e) - 1);
  }
  pair<ll, ll> get(int limit = 0) {
    while (true) {
      vector<ll> d(V, inf), p(V, -1);
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
      if (fl >= limit && limit) break;
    }
    return {fl, co};
  }

  vector<int> ord, us;

  ll dfs(int v, ll cfl = inf) {
    if (v == T)
      return (cfl != inf ? cfl : 0);
    us[v] = 1;
    for (int i : g[v]) {
      if (!us[e[i].to] && e[i].f > 0) {
        ord.push_back(e[i].id);
        ll d = dfs(e[i].to, min(cfl, e[i].f));
        if (d) {
          e[i].f -= d;
          e[e[i].link].f += d;
          return d;
        }
        ord.pop_back();
      }
    }
    return 0;
  }

  vector<vector<int>> decomp() {
    vector<vector<int>> ret;
    us.resize(V);
    while (fl--) {
      fill(us.begin(), us.end(), 0);
      assert(dfs(S));
      ret.push_back(ord);
      ord.clear();
    }
    return ret;
  }
};
