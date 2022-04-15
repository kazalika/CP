  /******************************************************

    Maximum matching in bipartite graph by Kazalika
       Kuhn's algorithm
       :time: O(nm) (but fast enough)

*******************************************************/

struct matching {
    int sz_left, sz_right, cused;
    vector<vector<int>> g;
    vector<int> tm, bck, used;

    matching() : sz_left(0), sz_right(0) {}
    matching(int sz_left_, int sz_right_, vector<vector<int>> &g_) : g(g_) {
        sz_left = sz_left_;
        sz_right = sz_right_;
        bck.resize(sz_right, -1);
        tm.resize(sz_left, -1);
        used.resize(sz_left);
        cused = 1;
    }

    bool kuhn(int v) {
        if (used[v] == cused)
            return false;
        used[v] = cused;
        for (int u : g[v]) {
            if (bck[u] == -1 || kuhn(bck[u])) {
                bck[u] = v;
                tm[v] = u;
                return true;
            }
        }
        return false;
    }
    int calc() {
        for (int run = 1; run; ) {
            run = 0, cused++;
            for (int v = 0; v < sz_left; ++v)
                if (tm[v] == -1 && kuhn(v))
                    run = 1;
        }
        int ans = 0;
        for (int x : tm)
            if (x != -1)
                ans++;
        return ans;
    }

    void add_edge(int a, int b) {
        g[a].push_back(b);
    }
};
