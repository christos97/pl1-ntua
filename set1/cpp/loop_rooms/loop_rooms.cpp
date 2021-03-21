#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <list>

using namespace std;

#define debug(x) cout << #x << " = " << x << endl
#define ins(set, vertex) set.insert(vertex)
typedef set<int> si; 
typedef list<int> li;

ifstream file;
string out;
int n, m; /* Grid Dimensions */
enum Direction { UP = 28, DOWN = 11, RIGHT = 25, LEFT = 19, OUT_OF_BOUNDS = 9999 };
si winnable; /* Once you reach you can win (already registered as winnable room) */
bool existsInSet(si& _set, int& _adj_node);

class Graph {
    private:
        int nodes;
        li *adjlist;
        vector<bool> visited;

    public:
        Graph() {
        }

        Graph (int nodes) { 
            adjlist = new li[nodes];
            visited.resize(nodes, false);
            this->nodes = nodes;
        }

        ~Graph () { 
            delete [] adjlist;
        }

        void addEdge (int src, int dst) {
            adjlist[src].push_back(dst);
        }

        void dfs (int src) {
            visited[src] = true;
            for (auto& adj_node : adjlist[src]) {
                if (adj_node == OUT_OF_BOUNDS) continue;
                if (existsInSet(winnable, adj_node)) { ins(winnable, src); continue; }
                if (!visited[adj_node]) 
                    dfs(adj_node);
            }
        } 

        void next () {
            fill(visited.begin(), visited.end(), false);
        }
};

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    file.open(argv[1]);
    file >> out; n = stoi(out);
    file >> out; m = stoi(out);
    Graph graph(n*m);
    int v = 0, row = -1;
    while (file >> out) {
        row++;
        for (int col = 0; col < m ; col++){
            int direction = out[col] - '9';
            bool perimeter = row == 0 || row == n-1 || col == 0 || col == m-1;
            if (perimeter) { /* Room with OUT_OF_BOUNDS edge is winnable */
                switch (direction){
                    case UP: {
                        if (row == 0) {
                            graph.addEdge(v, OUT_OF_BOUNDS);
                            ins(winnable, v);
                        }
                        else graph.addEdge(v, v - m);
                        break;
                    }
                    case DOWN: {
                        if (row == n-1) {
                            graph.addEdge(v, OUT_OF_BOUNDS);
                            ins(winnable, v);
                        }
                        else graph.addEdge(v, v + m);
                        break;
                    }
                    case LEFT: {
                        if (col == 0) {
                            graph.addEdge(v, OUT_OF_BOUNDS);
                            ins(winnable, v);
                        }
                        else graph.addEdge(v, v - 1);
                        break;
                    }
                    case RIGHT: {
                        if (col == m-1) {
                            graph.addEdge(v, OUT_OF_BOUNDS);
                            ins(winnable, v);
                        }
                        else graph.addEdge(v, v + 1);
                        break;
                    }
                    default: perror("invalid char"); exit(1);
                }
                v++;
            }
            else {
                switch (direction){
                    case UP: graph.addEdge(v, v - m); break;
                    case DOWN: graph.addEdge(v, v + m); break;
                    case LEFT: graph.addEdge(v, v - 1); break;
                    case RIGHT: graph.addEdge(v, v + 1); break;
                    default: perror("invalid char"); exit(1);
                }
                v++;
            }
        }
    } 
    file.close();
    for (int i=0; i < n*m; i++){
        graph.dfs(i); graph.next();
    }
    int doomed = n*m - winnable.size();
    cout<< doomed << endl;
}

inline bool existsInSet(si& _set, int& _adj_node){
    return _set.find(_adj_node) != _set.end();
}