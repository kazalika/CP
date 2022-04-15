using ll = long long;

const ll inf = 1e9 + 7;

/*******************************************
 
 Structure for finding maxflow by @Kzalika
 Dinic's Algorithm with scaling O(VE logC)
 
    +get_mincut
    +decomposition
 
*******************************************/

struct Edge {
    int from, to;
    ll f, c;
    int link;
    int id;
};

struct Way {
    ll f;
    vector<int> edges;
};

struct Flow {
    int size;
    int used_now;
    ll cur_flow;
    int S, T;   // S - source, T - runoff
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
    Flow(int sz, int S_, int T_) : size(sz) {
        S = S_, T = T_;
        used_now = 1, cur_flow = 0;
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
    void bfs(ll limit) {                 // finding the shortest ways from S
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
    ll dfs(int v, ll fl, ll limit) { // finding flow in residual network
        if (v == T)
            return fl;
        used[v] = used_now;
        for (Edge &e : g[v]) {
            if (used[e.to] == used_now || e.c - e.f < limit || del[e.to] || dist[e.to] != dist[v] + 1)
                continue;
            ll d = dfs(e.to, min(e.c - e.f, fl), limit);
            if (d) {
                e.f += d;
                g[e.to][e.link].f -= d;
                return d;
            }
        }
        del[v] = true;
        return 0;
    }

    ll Dinic(ll limit) {         // Dinic with limit
        bfs(limit);
        if (dist[T] == size)
            return 0;
        ll rt_flow = 0;
        fill(del.begin(), del.end(), false);
        while (!del[S]) {
            rt_flow += dfs(S, inf, limit);
            used_now++;
        }
        return rt_flow;
    }
    void Dinic() {                 // Dinic making full flow
        for (int i = 30; i >= 0; --i) {
            ll d = Dinic(1 << i);
            while (d) {
                cur_flow += d;
                d = Dinic(1 << i);
            }
        }
    }

    void color(int v) {
        used[v] = used_now;
        for (Edge &e : g[v]) {
            if (used[e.to] != used_now && e.f < e.c)
                color(e.to);
        }
    }
    vector<pair<int, int>> get_mincut() {
        vector<pair<int, int>> ret;
        Dinic();
        color(S);
        used_now++;
        for (int v = 0; v < size; ++v) {
            if (used[v] != used_now - 1)
                continue;
            for (Edge &e : g[v]) {
                if (used[e.to] != used[v]) {
                    ret.emplace_back(v, e.to);
                }
            }
        }
        return ret;
    }

    vector<int> way;

    ll find_way(int v, ll fl = inf) {
        if (v == T)
            return (fl == inf ? 0 : fl);
        used[v] = used_now;
        for (Edge &e : g[v]) {
            if (used[e.to] == used_now || e.f <= 0)
                continue;
            way.push_back(e.id);
            ll d = find_way(e.to, min(e.f, fl));
            if (d) {
                e.f -= d;
                g[e.to][e.link].f += d;
                return d;
            }
            way.pop_back();
        }
        cur_flow = 0;
        return 0;
    }

    vector<Way> Decomposition() {
        vector<Way> ret;
        ll d = find_way(S);
        while (d) {
            ret.push_back({d, way});
            way.clear();
            used_now++;
            d = find_way(S);
        }
        way.clear();
        used_now++;
        for (int i = 0; i < size; ++i) {
            d = find_way(i);
            while (d) {
                ret.push_back({d, way});
                way.clear();
                used_now++;
                d = find_way(i);
            }
            way.clear();
            used_now++;
        }
        return ret;
    }

    void ae(int a, int b, ll c, int id = -1) {  // add a->b with capacity = c
        g[a].push_back({a, b, 0, c, (int)g[b].size(), id});
        g[b].push_back({b, a, 0, 0, (int)g[a].size() - 1, id});
    }
    void ae2(int a, int b, ll c, int id = -1) { // add a<->b with capacity = c
        g[a].push_back({a, b, 0, c, (int)g[b].size(), id});
        g[b].push_back({b, a, -c, 0, (int)g[a].size() - 1, id});
    }
    void add_edge(int a, int b, ll c, int id = -1, int ts = 0) {    // add edge { a-b with capacity = c }
        if (a == -1) a = S;     // if S-b
        if (b == -1) b = T;     // if a-T
        if (ts) ae2(a, b, c, id);   // if a<->b
        else ae(a, b, c, id);       // if a->b
    }

};
