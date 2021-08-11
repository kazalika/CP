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
  int fr, to; ll co, f, c; int lk; // from, to, cost, flow, capacity, back_link
};
struct mcmf {
  int V, S, T;                     // num of vertexes, source = V-2, target = V-1
  vector<edge> e;                  // edges
  vector<vector<int>> g;           // graph
  ll fl, co;                       // current flow & cost
  mcmf() : V(0), fl(0), co(0), S(0), T(0) {}
  mcmf(int V_) : V(V_ + 2), fl(0), co(0), S(V_), T(V_ + 1) {}
  //             from     to    cost  capacity
  void add_edge(int fr, int to, ll cs, ll c) {       // fr = -1 => fr = S, to = -1 => to = T
    e.push_back({(fr == -1 ? S : fr), to, cs, 0, c, sz(e) + 1});
    e.push_back({(to == -1 ? T : to), fr, -cs, 0, 0, sz(e) - 1});
    g[fr].push_back(sz(e) - 2), g[to].push_back(sz(e) - 1);
  }
  void calc() {                    // calc flow & cost
    vector<int> d(V), p(V), vis(V), q(V << 2);       // dist, parent, visited, queue
    while (true) {
      fill(d.begin(), d.end(), inf);
      fill(p.begin(), p.end(), -1);                  // https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm
      d[S] = 0, q[0] = S;                            // SPFA (Shortest Path Faster Algorithm)
      int t = 1; ll af = INF;
      for (int i = 0; i != t; i = (i + 1) % (V << 2)) {
        int v = q[i];
        for (int j : g[v]) {
          auto ed = e[j];
          if (ed.f < ed.c && !vis[ed.to] && d[ed.to] > d[v] + ed.co)
            d[ed.to] = d[v] + ed.co, vis[ed.to] = 1, q[t] = ed.to, p[ed.to] = j, t = (t + 1) % (V << 2);
        }
        vis[v] = 0;
      }
      if (d[T] == inf) break;                       // shortest path doesn't exist -> *stop* 
      for (int v = T; v != S; v = e[p[v]].fr)
        af = min(af, e[p[v]].c - e[p[v]].f);
      for (int v = T; v != S; v = e[p[v]].fr)
        e[p[v]].f += af, e[e[p[v]].lk].f -= af, co += e[p[v]].co * af;
      fl += af;
    }
  }
};
