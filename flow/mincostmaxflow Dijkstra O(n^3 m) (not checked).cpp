/***********************************************

     Min cost max flow by Kazalika
     -O(n^3 m)
     -Dijkstra algorithm
     
     !  I don't recommend this algorithm     !
     !  because of edges with negative cost  !

************************************************/

const int inf = 1e9 + 9;

struct edge {
  int fr, to, w, f, c, link;
};

struct mcmf {
  int size, S{}, T{};
  vector<vector<edge>> g;
  int cur_flow, cur_cost;
  bool nupd;

  mcmf() : size(0), cur_flow(0), cur_cost(0), nupd(false) {}
  explicit mcmf(vector<vector<edge>> &g_) : g(g_), size(g_.size()), cur_flow(0), cur_cost(0), nupd(false) {
    size += 2;
    g.resize(size + 2);
    S = size_, T = size_ + 1;
  }
  explicit mcmf(int size_) : size(size_ + 2), g(vector<vector<edge>>(size_ + 2)), cur_flow(0), cur_cost(0), nupd(false) {    
    S = size_, T = size_ + 1; 
  }

  void ae1(int fr, int to, int w, int c) {
    g[fr].push_back({fr, to, w, 0, c, (int) g[to].size()});
    g[to].push_back({to, fr, -w, 0, 0, (int) g[fr].size() - 1});
  }
  void ae2(int fr, int to, int w, int c) {
    g[fr].push_back({fr, to, w, 0, c, (int) g[to].size()});
    g[to].push_back({to, fr, -w, -c, 0, (int) g[fr].size() - 1});
  }
  void add_edge(int fr, int to, int w, int c, int rev = 0) {
    if (fr == -1) fr = S;
    if (to == -1) to = T;
    if (rev) ae2(fr, to, w, c);
    else ae1(fr, to, w, c);
    nupd = true;
  }

  pair<int, int> calc() {
    if (!nupd)
      return {cur_flow, cur_cost};
    int flow = 0, cost = 0;
    while (true) {
      vector<int> dist(size, inf), p(size, -1), ped(size, -1);
      set<pair<int, int>> q = {{0, S}};
      dist[S] = 0;
      while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());
        for (int i = 0; i < g[v].size(); ++i) {
          auto ed = g[v][i];
          if (ed.f < ed.c && dist[ed.to] > dist[v] + ed.w) {
            dist[ed.to] = dist[v] + ed.w, p[ed.to] = v, ped[ed.to] = i;
            q.insert({dist[ed.to], ed.to});
          }
        }
      }
      if (dist[T] == inf)
        break;
      int af = inf;
      for (int v = T; v != S; v = p[v])
        af = min(af, g[p[v]][ped[v]].c - g[p[v]][ped[v]].f);
      for (int v = T; v != S; v = p[v]) {
        g[p[v]][ped[v]].f += af;
        g[v][g[p[v]][ped[v]].link].f -= af;
        cost += af * g[p[v]][ped[v]].w;
      }
      flow += af;
    }
    cur_flow += flow, cur_cost += cost;
    nupd = false;
    return {cur_flow, cur_cost};
  }

};
