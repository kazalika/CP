/******************************************************

    Maximum matching in bipartite graph by Kazalika
       Kuhn's algorithm
       :time: O(nm)

*******************************************************/

struct matching {
  int sz_left, sz_right, size, cused;
  vector<vector<int>> g;
  vector<int> bck, used, lg, rg;

  matching() : sz_left(0), sz_right(0), size(0) {}
  matching(vector<vector<int>> &g_, vector<int> &lg_, vector<int> &rg_) : g(g_), lg(lg_), rg(rg_) {
    size = g.size();
    sz_left = lg.size();
    sz_right = rg.size();
    bck.resize(size, -1);
    used.resize(size);
    cused = 1;
  }

  bool kuhn(int v) {
    if (used[v] == cused)
      return false;
    used[v] = cused;
    for (int u : g[v]) {
      if (bck[u] == -1 || kuhn(bck[u])) {
        bck[u] = v;
        return true;
      }
    }
    return false;
  }
  int calc() {
    int ans = 0;
    for (int v : lg) {
      if (kuhn(v))
        ans++;
      cused++;
    }
    return ans;
  }

  void add_edge(int a, int b) {
    g[a].push_back(b);
  }
};
