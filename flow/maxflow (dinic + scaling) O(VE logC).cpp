struct Edge {
  int from, to, f, c, link;
};

struct Flow {
  int size, used_now, cur_flow;
  int S, T;
  vector<int> dist, used;
  vector<bool> del;
  vector<vector<Edge>> g;
  Flow() {}
  Flow(int sz) : size(sz) {
    S = size, T = size + 1;
    used_now = 1, cur_flow = 0;
    size += 2;
    g.resize(size);
    dist.resize(size);
    used.resize(size);
    del.resize(size);
  }
  void resize(int sz) {
    size = sz;
    S = size, T = size + 1;
    used_now = 1, cur_flow = 0;
    size += 2;
    g.resize(size);
    dist.resize(size);
    used.resize(size);
    del.resize(size);
  }
  void bfs(int limit) {
    fill(dist.begin(), dist.end(), size);
    dist[S] = 0;
    vector<int> q;
    q.push_back(S);
    for (int i = 0; i < q.size(); ++i) {
      int v = q[i];
      for (Edge &e : g[v]) {
        if (e.c - e.f >= limit && dist[e.to] > dist[v] + 1) {
          dist[e.to] = dist[v] + 1;
          q.push_back(e.to);
        }
      }
    }
  }
  int dfs(int v, int fl, int limit) {
    if (v == T)
      return fl;
    used[v] = used_now;
    for (Edge &e : g[v]) {
      if (used[e.to] == used_now || e.c - e.f < limit || del[e.to] || dist[e.to] != dist[v] + 1)
        continue;
      int d = dfs(e.to, min(e.c - e.f, fl), limit);
      if (d) {
        e.f += d;
        g[e.to][e.link].f -= d;
        return d;
      }
    }
    del[v] = true;
    return 0;
  }

  int Dinic(int limit) {
    bfs(limit);
    if (dist[T] == size)
      return 0;
    int rt_flow = 0;
    fill(del.begin(), del.end(), false);
    while (!del[S]) {
      rt_flow += dfs(S, inf, limit);
      used_now++;
    }
    return rt_flow;
  }
  void Dinic() {
    for (int i = 30; i >= 0; --i) {
      int d = Dinic(1 << i);
      while (d) {
        cur_flow += d;
        d = Dinic(1 << i);
      }
    }
  }

  void ae(int a, int b, int c) {
    g[a].push_back({a, b, 0, c, (int)g[b].size()});
    g[b].push_back({b, a, 0, 0, (int)g[a].size() - 1});
  }
  void ae2(int a, int b, int c) {
    g[a].push_back({a, b, 0, c, (int)g[b].size()});
    g[b].push_back({b, a, -c, 0, (int)g[a].size() - 1});
  }
  void add_edge(int a, int b, int c, int ts = 0) {    // add edge { a->b with limit = c }    if S->b: a = -1; if a->T: b = -1;
    if (a == -1) a = S;
    if (b == -1) b = T;
    if (ts) ae2(a, b, c);
    else ae(a, b, c);
  }

};
