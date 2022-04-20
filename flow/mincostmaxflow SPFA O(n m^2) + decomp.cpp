/***********************************

    Min cost max flow by Kazalika
    -O(n m^2) (not always)
    -SPFA

************************************/

const int inf = 1e9 + 9;
typedef long long ll;
const ll INF = 1e15;
#define sz(x) (int)(x).size()

struct edge {
  int fr, to; ll co, f, c; int lk, id; // from, to, cost, flow, capacity, back_link, edge_index
};
struct mcmf {
  int V, S, T;                     // num of vertexes, source = V-2, target = V-1
  vector<edge> e;                  // edges
  vector<vector<int>> g;           // graph
  ll fl, co;                       // current flow & cost
  mcmf() : V(0), fl(0), co(0), S(0), T(0) {}
  mcmf(int V_) : V(V_ + 2), fl(0), co(0), S(V_), T(V_ + 1), g(vector<vector<int>>(V_ + 2)) {}
  mcmf(int V_, int S_, int T_) : V(V_), fl(0), co(0), S(S_), T(T_), g(vector<vector<int>>(V_)) {}

  //             from     to    cost  capacity  edge_index
  void add_edge(int fr, int to, ll cs, ll c, int id = 0) {       // fr = -1 => fr = S, to = -1 => to = T
    if (fr == -1) fr = S;
    if (to == -1) to = T;
    e.push_back({fr, to, cs, 0, c, sz(e) + 1, id});
    e.push_back({to, fr, -cs, 0, 0, sz(e) - 1, id});
    g[fr].push_back(sz(e) - 2), g[to].push_back(sz(e) - 1);
  }
  void calc(int limit = 0) {                    // calc flow & cost
    vector<ll> d(V);
    vector<int> p(V), vis(V), q(V << 2);       // dist, parent, visited, queue
    while (true) {
      fill(vis.begin(), vis.end(), 0);
      fill(d.begin(), d.end(), INF);
      fill(p.begin(), p.end(), -1);                  // https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm
      d[S] = 0, q[0] = S;                            // SPFA (Shortest Path Faster Algorithm)
      int t = 1; ll af = INF;
      for (int i = 0; i != t; i = (i + 1) % (V << 2)) {
        int v = q[i];
        for (int j : g[v]) {
          auto ed = e[j];
          if (ed.f < ed.c && d[ed.to] > d[v] + ed.co) {
            d[ed.to] = d[v] + ed.co, p[ed.to] = j;
            if (!vis[ed.to])
              vis[ed.to] = 1, q[t] = ed.to, t = (t + 1) % (V << 2);
          }
        }
        vis[v] = 0;
      }
      if (d[T] == INF) break;                       // shortest path doesn't exist -> *stop*
      for (int v = T; v != S; v = e[p[v]].fr)
        af = min(af, e[p[v]].c - e[p[v]].f);
      for (int v = T; v != S; v = e[p[v]].fr)
        e[p[v]].f += af, e[e[p[v]].lk].f -= af, co += e[p[v]].co * af;
      fl += af;
      if (fl >= limit && limit) break;
    }
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
          e[e[i].lk].f += d;
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
