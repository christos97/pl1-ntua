#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <list>
#include <stack>

using namespace std;
#define debug(x) cout << #x << " = " << x << endl

ifstream file;
string out;
set<int> winnable; // Once you reach you can win (already registered as winnable room)
set<int> blacklist; // Once you reach you can't win (already registered as not winnable room)
enum Direction { UP = 28, DOWN = 11, RIGHT = 25, LEFT = 19, OUT_OF_BOUNDS = 9999 };
int n, m;
bool existsInWinnable(int& next);
//bool isInBlacklist(int& current);
class Graph {
    private:
        int nodes;
        list<int> *adjlist;
        vector<bool> visited;

    public:
        Graph() {
        }

        Graph (int nodes) { 
            adjlist = new list<int> [nodes];
            visited.resize(nodes, false);
            this->nodes = nodes;
        }

        ~Graph () { 
            delete [] adjlist;
        }

        void addEdge (int src, int dst) {
            adjlist[src].push_back(dst);
        }


        // DFS recursive
        void dfs (int src) {
            visited[src] = true;
            for (auto& adj_node : adjlist[src]) {
                if (adj_node == OUT_OF_BOUNDS) continue;
                if (existsInWinnable(adj_node)) { /* If neighboor is winnable, src is winnable too */
                    winnable.insert(src);
                    debug(src);
                    debug(adj_node);
                    cout << endl;
                    continue;
                }
                if (!visited[adj_node]) {
                    dfs(adj_node);
                }
            }
        } 

    // DFS iterative
        void dfsIterative (int src) {
            stack<int> stk;
            visited[src] = true;
            stk.push(src);
            while (!stk.empty()) {
                src = stk.top();
                stk.pop();
                for (auto &adj_node : adjlist[src]) {
                    if (adj_node == OUT_OF_BOUNDS) continue; // perimeter
                    if (existsInWinnable(adj_node)) {
                        winnable.insert(src);
                        debug(adj_node);
                        continue;
                    }
                    if (!visited[adj_node]) {
                        visited[adj_node] = true;
                        stk.push(adj_node);
                    }
                }
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
    int vertex = 0, row = -1;
    while (file >> out) {
        row++;
        for (int col = 0; col < m ; col++){
            int direction = out[col] - '9';
            bool perimeter = row == 0 || row == n-1 || col == 0 || col == m-1;
            if (perimeter) { /* Room with OUT_OF_BOUNDS edge is winnable */
                switch (direction){
                    case UP: {
                        if (row == 0) {
                            graph.addEdge(vertex, OUT_OF_BOUNDS);
                            winnable.insert(vertex);
                        }
                        else graph.addEdge(vertex, vertex - m);
                        break;
                    }
                    case DOWN: {
                        if (row == n-1) {
                            graph.addEdge(vertex, OUT_OF_BOUNDS);
                            winnable.insert(vertex);
                        }
                        else graph.addEdge(vertex, vertex + m);
                        break;
                    }
                    case LEFT: {
                        if (col == 0) {
                            graph.addEdge(vertex, OUT_OF_BOUNDS);
                            winnable.insert(vertex);
                        }
                        else graph.addEdge(vertex, vertex - 1);
                        break;
                    }
                    case RIGHT: {
                        if (col == m-1) {
                            graph.addEdge(vertex, OUT_OF_BOUNDS);
                            winnable.insert(vertex);
                        }
                        else graph.addEdge(vertex, vertex + 1);
                        break;
                    }
                    default: perror("invalid char"); exit(1);
                }
                vertex++;
            }
            else {
                switch (direction){
                    case UP: graph.addEdge(vertex, vertex - m); break;
                    case DOWN: graph.addEdge(vertex, vertex + m); break;
                    case LEFT: graph.addEdge(vertex, vertex - 1); break;
                    case RIGHT: graph.addEdge(vertex, vertex + 1); break;
                    default: perror("invalid char"); exit(1);
                }
                vertex++;
            }
        }
    } 
    file.close();
    
    for (int i=0; i < n*m; i++){
        graph.dfs(i);
        graph.next();
    }

    int doomed = n*m - winnable.size();
    debug(doomed);
}

/* bool isInBlacklist(int& current) {
    return blacklist.find(current) != blacklist.end();
} */

inline bool existsInWinnable(int& next){
    return winnable.find(next) != winnable.end();
}