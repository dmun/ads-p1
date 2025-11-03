#include <algorithm>
#include <iostream>
#include <stack>
using namespace std;

struct Graph {
    int num_vertices;
    vector<vector<pair<int, int>>> adj;

  public:
    Graph(int vertices) : num_vertices(vertices) { adj.resize(vertices); }

    void add_edge(int v, int u, int value) {
        adj[v].push_back(make_pair(u, value));
        adj[u].push_back(make_pair(v, value));
    }

    bool is_valid() {
        vector<bool> valid_vertices(num_vertices, false);

        for (int v = 0; v < num_vertices; v++) {
            vector<bool> explored(num_vertices, false);

            stack<int> frontier;
            for (int i = 0; i < num_vertices && frontier.empty(); i++) {
                auto neighbor = adj[v][i];
                if (neighbor.second == 2) {
                    frontier.push(neighbor.first);
                }
            }

            if (frontier.empty()) {
                frontier.push(0);
            }

            while (!frontier.empty()) {
                int u = frontier.top();
                frontier.pop();

                for (auto w : adj[u]) {
                    if (!explored[w.first]) {
                        explored[w.first] = true;
                        frontier.push(w.first);
                    }
                }
            }

            valid_vertices[v] = all_of(explored.begin(), explored.end(),
                                       [](bool b) { return b; });
        }

        return all_of(valid_vertices.begin(), valid_vertices.end(),
                      [](bool b) { return b; });
    }

    void printGraph() {
        for (int i = 0; i < num_vertices; i++) {
            cout << i << " - ";
            for (auto neighbor : adj[i]) {
                cout << neighbor.first << "(" << neighbor.second << ") ";
            }
            cout << "\n";
        }
    }
};

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

    P.printGraph();
    B.printGraph();
    cout << "is_valid(): " << P.is_valid() << endl;

    return 0;
}
