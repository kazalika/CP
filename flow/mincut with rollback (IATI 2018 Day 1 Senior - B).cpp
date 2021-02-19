#include <bits/stdc++.h>
using namespace std;

//#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,popcnt,abm,mmx,avx,tune=native")
//#pragma GCC target("avx,avx2")

const int inf = 1e9 + 9;

/*******************************************
 Structure for finding maxflow by @Kzalika
        Dinic's Algorithm O(EV^2)
*******************************************/

struct Edge {
  int from, to, f, c, link;
};
struct Ins {
  int a, b, c, ts;
};
struct Added {
  int v, num_ed, del;
};

struct Flow {
  int size, used_now, cur_flow;
  int S, T;   // S - source, T - runoff
  vector<int> dist, used, szedge;
  vector<bool> del;
  vector<vector<Edge>> g;
  vector<vector<Added>> story;  // vector of added changes in graph
  vector<pair<int, int>> edges; // vector of added edges
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
  void bfs(int limit) {                 // finding the shortest ways from S
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
  int dfs(int v, int fl, int limit) { // finding flow in residual network
    if (v == T)
      return fl;
    used[v] = used_now;
    for (Edge &e : g[v]) {
      if (used[e.to] == used_now || e.c - e.f < limit || del[e.to] || dist[e.to] != dist[v] + 1)
        continue;
      int d = dfs(e.to, min(e.c - e.f, fl), limit);
      if (d) {
        int id = g[e.to][e.link].link;
        story.back().push_back({v, id, d});
        e.f += d;
        g[e.to][e.link].f -= d;
        return d;
      }
    }
    del[v] = true;
    return 0;
  }

  int Dinic(int limit) {         // Dinic with limit
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
  void Dinic() {                 // Dinic making full flow
//    while (Dinic(1));
    for (int i = 30; i >= 0; --i) {
      while (Dinic(1 << i));
    }
  }

  void ae(int a, int b, int c) {  // add a->b with capacity = c
    g[a].push_back({a, b, 0, c, (int)g[b].size()});
    g[b].push_back({b, a, 0, 0, (int)g[a].size() - 1});
  }
  void ae2(int a, int b, int c) { // add a<->b with capacity = c
    g[a].push_back({a, b, 0, c, (int)g[b].size()});
    g[b].push_back({b, a, -c, 0, (int)g[a].size() - 1});
  }
  void add_edge(int a, int b, int c, int ts = 0) {    // add edge { a-b with capacity = c }
    if (a == -1) a = S;     // if S-b
    if (b == -1) b = T;     // if a-T
    if (ts) ae2(a, b, c);   // if a<->b
    else ae(a, b, c);       // if a->b
    edges.emplace_back(a, b);
  }
  void insert_edge(vector<Ins>& vE) { // vE = vector of { a, b, c, ts } for add_edge(a, b, c, d) ^
    szedge.push_back(vE.size());
    for (auto &E : vE)
      add_edge(E.a, E.b, E.c, E.ts);
    story.emplace_back();
    Dinic();
  }
  void pop_edge() {
    for (auto &x : story.back()) {
      auto &e = g[x.v][x.num_ed];
      e.f -= x.del;
      g[e.to][e.link].f += x.del;
    }
    for (int i = 0; i < szedge.back(); ++i) {
      g[edges.back().first].pop_back();
      g[edges.back().second].pop_back();
      edges.pop_back();
    }
    szedge.pop_back();
    story.pop_back();
  }
  int get_flow() {     // get cur max flow O(deg S)
    cur_flow = 0;
    for (auto &e : g[S])
      cur_flow += e.f;
    return cur_flow;
  }

  void clear() {       // delete (clear) the graph
    size = 0;
    g.clear();
    del.clear();
    dist.clear();
    used.clear();
    story.clear();
    used_now = cur_flow = 0;
    S = T = -1;
  }
};

struct drug {
  int x, y, t;
};

Flow F;

const int N = 2e3 + 3;

int n, m, q;

int good[N], bad[N], small;
vector<drug> druzja;
vector<int> types;
vector<Ins> queries;

bool exist[N][N];

int ans[N];

struct segment_tree {
  int size;
  vector<vector<Ins>> tr;
  segment_tree() {}
  segment_tree(int sz) : size(sz) {
    tr.resize(4 * size);
  }
  void add(int t, int l, int r, int x, int y, Ins E) {
    if (l >= y || r <= x)
      return;
    if (l >= x && r <= y) {
      tr[t].push_back(E);
      return;
    }
    int md = r + l >> 1;
    add(t * 2 + 1, l, md, x, y, E);
    add(t * 2 + 2, md, r, x, y, E);
  }
  void add(int x, int y, Ins E) { add(0, 0, size, x, y + 1, E); }

  void dfs(int t, int l, int r) {
    F.insert_edge(tr[t]);
    if (l + 1 == r) {
      small = 0;
      for (int i = 0; i < n; ++i)
        if (exist[l][i])
          small += good[i] + bad[i];
      ans[l] = small - F.get_flow();
    } else {
      int md = r + l >> 1;
      dfs(t * 2 + 1, l, md);
      dfs(t * 2 + 2, md, r);
    }
    F.pop_edge();
  }
  void dfs() { dfs(0, 0, size); }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  F.resize(n);
  for (int i = 0; i < n; ++i) {
    cin >> good[i];
    queries.push_back({-1, i, good[i], 0});
  }
  for (int i = 0; i < n; ++i) {
    cin >> bad[i];
    queries.push_back({i, -1, bad[i], 0});
  }
  for (int i = 0; i < m; ++i) {
    int x, y, t;
    cin >> x >> y >> t;
    x--, y--;
    druzja.push_back({x, y, t});
    queries.push_back({x, y, t, 1});
  }
  cin >> q;
  q++;
  fill(exist[0], exist[0] + n, true);
  fill(exist[1], exist[1] + n, true);
  types.resize(q);
  for (int i = 1; i < q; ++i) {
    int t, x;
    cin >> t;
    types[i] = t;
    if (t == 1) {
      cin >> x;
      exist[i][x - 1] = true;
    }
    if (t == 2) {
      cin >> x;
      exist[i][x - 1] = false;
    }
    if (t == 3) {
      fill(exist[i], exist[i] + n, true);
    }
    if (t == 4) {
      fill(exist[i], exist[i] + n/5, false);
    }
    for (int j = 0; j < n; ++j)
      exist[i + 1][j] = exist[i][j];
  }

  segment_tree sgt(q);

  for (auto &E : queries) {
    int l = -1;
    for (int i = 0; i < q; ++i) {
      bool ex = ((E.a == -1 || exist[i][E.a]) && (E.b == -1 || exist[i][E.b]));
      if (ex) {
        if (l == -1)
          l = i;
      } else {
        if (l != -1)
          sgt.add(l, i - 1, E);
        l = -1;
      }
    }
    if (l != -1)
      sgt.add(l, q - 1, E);
  }
  for (int i = 0; i < n; ++i)
    small += good[i] + bad[i];
  sgt.dfs();
  for (int i = 0; i < q; ++i) {
    if (types[i] <= 2) {
      cout << ans[i] << '\n';
    }
  }
}
