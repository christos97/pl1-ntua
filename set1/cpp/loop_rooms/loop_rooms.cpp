#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <list>
#include <chrono>
using namespace std;
#define debug(x) cout << #x << " = " << x << endl
typedef set<int> si; 
typedef list<int> li;

enum Direction { UP = 28, DOWN = 11, RIGHT = 25, LEFT = 19, OUT_OF_BOUNDS = 9999 }; 
int n, m; /* Grid size */
si winnable; /* Once you reach you can win (already registered as winnable room) */
bool existsInSet(si& _set, int& _v);

class Graph {
    private:
        int nodes;
        li *adjlist;
        vector<bool> visited;
        void addEdge(int _src, int _dst);
        void dfs(int _src);
        void next();

    public:
        Graph() { }
        Graph (int _nodes) { 
            adjlist = new li[_nodes];
            visited.resize(_nodes, false);
            this->nodes = _nodes;
        }
        ~Graph () { delete [] adjlist; }
        void from(ifstream& _file);
        int countDoomed();
};

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    auto start = chrono::steady_clock::now();

    ifstream file;
    string gridSize;
    file.open(argv[1]);
    file >> gridSize; n = stoi(gridSize);
    file >> gridSize; m = stoi(gridSize);
    Graph graph(n*m);
    graph.from(file);
    file.close();
    int doomed = graph.countDoomed();
    debug(doomed);
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
    for (auto winab: winnable) debug(winab);
}

inline bool existsInSet(si& _set, int& _v) {
    return _set.find(_v) != _set.end();
}

/* Private */
void Graph::addEdge (int _src, int _dst) {
    adjlist[_src].push_back(_dst);
}

void Graph::dfs (int _src) {
    visited[_src] = true;
    for (auto& adj_node : adjlist[_src]) {
        if (adj_node == OUT_OF_BOUNDS) continue; /* Perimeter is checked at file reading */
        if (existsInSet(winnable, adj_node)) { /* My neighboor can win, so i can win too */
            for (size_t i = 0; i <= visited.size(); i++) {
                if (visited[i]) winnable.insert(i); 
            } 
            continue; 
        }
        if (!visited[adj_node]) 
            dfs(adj_node);
    }
} 

void Graph::next () {
    fill(visited.begin(), visited.end(), false);
}

/* Public */
void Graph::from(ifstream& _file) {
    int row = -1, v = 0;
    string out;
    while (_file >> out) {
        row++;
        for (int col = 0; col < m ; col++){
            switch (out[col] - '9'){
                case UP: {
                    if (row == 0) { addEdge(v, OUT_OF_BOUNDS); winnable.insert(v); }
                    else addEdge(v, v - m);
                    break;
                }
                case DOWN: {
                    if (row == n-1) { addEdge(v, OUT_OF_BOUNDS); winnable.insert(v); }
                    else addEdge(v, v + m);
                    break;
                }
                case LEFT: {
                    if (col == 0) { addEdge(v, OUT_OF_BOUNDS); winnable.insert(v); }
                    else addEdge(v, v - 1);
                    break;
                }
                case RIGHT: {
                    if (col == m-1) { addEdge(v, OUT_OF_BOUNDS); winnable.insert(v); }
                    else addEdge(v, v + 1);
                    break;
                }
                default: perror("invalid char"); exit(1);
            }
            v++;
        }
    }
}

int Graph::countDoomed() {
    for (int i=0; i < n*m; i++){ dfs(i); next(); }
    int winab_count = winnable.size();
    debug(n*m);
    debug(winab_count);
    return n*m - winnable.size();
}