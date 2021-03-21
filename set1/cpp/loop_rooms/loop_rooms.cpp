#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <list>
using namespace std;
#define debug(x) cout << #x << " = " << x << endl
#define ins(_set, _v) _set.insert(_v)
typedef set<int> si; 
typedef list<int> li;

ifstream file;
string out;
int n, m; /* Grid Dimensions */
enum Direction { UP = 28, DOWN = 11, RIGHT = 25, LEFT = 19, OUT_OF_BOUNDS = 9999 }; /* So we don't hurt our eyes */
si winnable; /* Once you reach you can win (already registered as winnable room) */
bool existsInSet(si& _set, int& _v);

class Graph {
    private:
        int nodes;
        li *adjlist;
        vector<bool> visited;

    public:
        Graph() { }
        Graph (int _nodes) { 
            adjlist = new li[_nodes];
            visited.resize(_nodes, false);
            this->nodes = _nodes;
        }
        ~Graph () { delete [] adjlist; }
        void addEdge(int _src, int _dst);
        void dfs(int _src);
        void next();
};

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    file.open(argv[1]);
    file >> out; n = stoi(out);
    file >> out; m = stoi(out);
    Graph graph(n*m);
    int v = 0, row = -1; /* Every cell grid (vertex) has its own integer numbering */
    while (file >> out) {
        row++;
        for (int col = 0; col < m ; col++){
            int direction = out[col] - '9';
            bool perimeter = row == 0 || row == n-1 || col == 0 || col == m-1;
            switch (direction){
                case UP: {
                    if (row == 0 && perimeter) { graph.addEdge(v, OUT_OF_BOUNDS); ins(winnable, v); }
                    else graph.addEdge(v, v - m);
                    break;
                }
                case DOWN: {
                    if (row == n-1 && perimeter) { graph.addEdge(v, OUT_OF_BOUNDS); ins(winnable, v); }
                    else graph.addEdge(v, v + m);
                    break;
                }
                case LEFT: {
                    if (col == 0 && perimeter) { graph.addEdge(v, OUT_OF_BOUNDS); ins(winnable, v); }
                    else graph.addEdge(v, v - 1);
                    break;
                }
                case RIGHT: {
                    if (col == m-1 && perimeter) { graph.addEdge(v, OUT_OF_BOUNDS); ins(winnable, v); }
                    else graph.addEdge(v, v + 1);
                    break;
                }
                default: perror("invalid char"); exit(1);
            }
            v++;
        }
    } 
    file.close();

    /* Solve */
    for (int i=0; i < n*m; i++){
        graph.dfs(i); graph.next();
    }
    int doomed = n*m - winnable.size();
    cout<< doomed << endl;
}


void Graph::addEdge (int _src, int _dst) {
            adjlist[_src].push_back(_dst);
        }

void Graph::dfs (int _src) {
            visited[_src] = true;
            for (auto& adj_node : adjlist[_src]) {
                if (adj_node == OUT_OF_BOUNDS) continue; /* Perimeter is checked at file reading */
                if (existsInSet(winnable, adj_node)) { ins(winnable, _src); continue; } /* If my neighboor is winnable, im winnable */
                if (!visited[adj_node]) 
                    dfs(adj_node);
            }
        } 
void Graph::next () {
            fill(visited.begin(), visited.end(), false);
        }

inline bool existsInSet(si& _set, int& _v){
    return _set.find(_v) != _set.end();
}