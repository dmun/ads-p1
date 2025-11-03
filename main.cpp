#include <iostream>
#include <stack>
#include <vector>
using namespace std;

struct Graph {
    int num_vertices;
    vector<vector<pair<int, int>>> adj;

    Graph(int vertices) : num_vertices(vertices) { adj.resize(vertices); }

    void add_edge(int v, int u, int value) {
        adj[v].push_back({u, value});
        adj[u].push_back({v, value});
    }

    int get_edge_count() {
        int n = 0;
        for (int v = 0; v < num_vertices; v++) {
            for (auto neighbor : adj[v]) {
                int u = neighbor.first;
                if (v < u)
                    n++;
            }
        }
        return n;
    }

    bool is_connected() {
        vector<bool> explored(num_vertices, false);

        stack<int> frontier;
        frontier.push(0);
        int n = 0;

        while (!frontier.empty()) {
            int u = frontier.top();
            frontier.pop();

            for (auto w : adj[u]) {
                if (!explored[w.first]) {
                    explored[w.first] = true;
                    frontier.push(w.first);
                    n++;
                }
            }
        }

        return n == num_vertices;
    }

    void log() {
        cout << "edges: " << get_edge_count() << endl;
        for (int i = 0; i < num_vertices; i++) {
            cout << i << " - ";
            for (auto neighbor : adj[i]) {
                cout << neighbor.first << "(" << neighbor.second << ") ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

struct UnionFind {
    vector<int> parent;
    int num_vertices;

    UnionFind(int n) : num_vertices(n) {
        parent.resize(n);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int v) {
        while (parent[v] != v)
            v = parent[v];
        return v;
    }

    void unify(int v, int u) {
        int root_v = find(v);
        int root_u = find(u);
        parent[root_u] = root_v;
    }

    bool connected(int v, int u) { return find(v) == find(u); }
};

Graph get_mst(const Graph &g) {
    vector<pair<int, int>> edges0;
    vector<pair<int, int>> edges1;
    Graph mst(g.num_vertices);

    for (int v = 0; v < g.num_vertices; v++) {
        for (auto neighbor : g.adj[v]) {
            int u = neighbor.first;
            int value = neighbor.second;
            if (v < u) {
                if (value == 2)
                    edges0.push_back({v, u});
                else
                    edges1.push_back({v, u});
            }
        }
    }

    UnionFind uf(g.num_vertices);

    for (auto edge : edges0) {
        if (!uf.connected(edge.first, edge.second)) {
            mst.add_edge(edge.first, edge.second, 0);
            uf.unify(edge.first, edge.second);
        }
    }

    for (auto edge : edges1) {
        if (!uf.connected(edge.first, edge.second)) {
            mst.add_edge(edge.first, edge.second, 1);
            uf.unify(edge.first, edge.second);
        }
    }

    return mst;
}

int get_merged_edge_count(const Graph &g0, const Graph &g1) {
    int n = 0;

    for (int v = 0; v < g0.num_vertices; v++) {
        for (auto neighbor : g0.adj[v]) {
            int u = neighbor.first;
            if (v < u)
                n++;
        }
    }

    for (int v = 0; v < g1.num_vertices; v++) {
        for (auto neighbor : g1.adj[v]) {
            int u = neighbor.first;
            if (v < u && neighbor.second != 0)
                n++;
        }
    }

    return n;
}

int main(int argc, char *argv[]) {
    int S, R;
    cin >> S >> R;

    Graph P(S); // pedestrian + both roads
    Graph B(S); // bus + both roads

    int s0, s1, T;
    while (cin >> s0 >> s1 >> T) {
        switch (T) {
        case 0:
            P.add_edge(s0, s1, T);
            break;
        case 1:
            B.add_edge(s0, s1, T);
            break;
        case 2:
            P.add_edge(s0, s1, T);
            B.add_edge(s0, s1, T);
            break;
        }
    }

    if (!P.is_connected() || !B.is_connected()) {
        cout << -1 << endl;
    } else {
        int n = get_merged_edge_count(get_mst(P), get_mst(B));
        cout << R - n << endl;
    }

    return 0;
}
