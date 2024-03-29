#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <list>
#include <chrono>
using namespace std;
#define debug(x) cout << #x << " = " << x << endl
#define now() chrono::steady_clock::now()
#define execTime(dt) cout << chrono::duration <double, milli> (dt).count() << " ms" << endl
#define append(src, dst) dst.insert(src.begin(), src.end())
typedef set<int> si; 
typedef list<int> li;

enum Direction { UP = 28, DOWN = 11, RIGHT = 25, LEFT = 19, OUT_OF_BOUNDS = 9999, DUMMY_PARENT = -9999 }; /* Acceptable moves */
int n; /* Grid rows */ 
int m; /* Grid cols */
bool existsInSet(si& _set, int& _v);

class Graph {
    private:
        int nodes; /* Number pf nodes */
        li *adjlist; /* Graph's adjacency list */
        vector<bool> visited; /* For the 'traditional' part of the dfs */
        si winnable; /* Once you reach you can win */
        si doomed; /* Once you reach you can't win */
        si visited_set; /* Temp memory for only current round's visited nodes for faster append to winnable or doomed set respectively in dfs */
        void addEdge(int _src, int _dst);
        void dfs(int& _src, int& _par);
        void next();

    public:
        Graph() { }
        Graph (int _nodes);
        ~Graph () { delete [] adjlist; }
        void from(ifstream& _file);
        int solve();
        void printSets();
};

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    //auto start = now();
    
    ifstream file;
    string gridSize;
    file.open(argv[1]);
    file >> gridSize; n = stoi(gridSize);
    file >> gridSize; m = stoi(gridSize);
    Graph graph(n*m);
    graph.from(file);
    file.close();
    int res = graph.solve();
    cout << res << endl;

    //graph.printSets();
    //execTime(now() - start);
}

/** 
 *  Search if value exists in set
 *  @param _set: Set to search integer value
 *  @param _v: Value to search for
 *  @returns true if found, false otherwise
*/
inline bool existsInSet(si& _set, int& _v) {
    return _set.find(_v) != _set.end();
}

/******************* Private Methods *******************/

/** 
 *  Create a directed edge between two nodes 
 *  @param _src: from node  
 *  @param _dst: to node
 *  @returns void
*/
void Graph::addEdge(int _src, int _dst) {
    adjlist[_src].push_back(_dst);
}

/** 
 * Recursive dfs that also searches for values in winnable and/or doomed sets before continuing.
 *  @param _src: source node to dfs 
 *  @param _par: parent of current source node
 *  @return void    
*/
void Graph::dfs(int& _src, int& _par) {
    visited[_src] = true; visited_set.insert(_src);
    for (int& adj_node : adjlist[_src]) {
        if (adj_node == OUT_OF_BOUNDS || adj_node == DUMMY_PARENT) continue; /* Skip these */
        if (adj_node == _par) { append(visited_set, doomed); continue; } /* I looped, we are all doomed */
        if (existsInSet(winnable, adj_node)) { append(visited_set, winnable); break; } /* I reached a winnable room, we can all win */
        if (existsInSet(doomed, adj_node)) { append(visited_set, doomed); break; } /* I reached a doomed room, we are all doomed */
        if (!visited[adj_node]) dfs(adj_node, _src); /* we still need the vector for 0(1) here, existsInSet(visited, adj_node) would be overkill */
    }
} 

/* Clear visited memory for next round */
void Graph::next() {
    fill(visited.begin(), visited.end(), false);
    visited_set.clear();
}

/******************* Public Methods *******************/

/**
 *  Class constructor, initialize private variables based on number of nodes given
 *  @param _nodes Number of nodes in the graph 
*/
Graph::Graph(int _nodes) {
    adjlist = new li[_nodes]; 
    visited.resize(_nodes, false);
    this->nodes = _nodes;
}

/** 
 *  Read file and create adjacency list 
 *  @param _file: File to create graph from
 *  @returns void or exits program if file invalid 
*/
void Graph::from(ifstream& _file) {
    int row = 0, v = 0;
    string out;
    while (_file >> out) { /* File has no spaces so every loop reads one row */
        for (int col = 0; col < m ; col++){ /* Pick a char */
            switch (out[col] - '9'){ /* Switch direction and save winnable rooms in grid perimeter */
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
        row++;
    }
}

/**
 *  Call after graph is created from file
 *  @returns Number of doomed rooms in grid  
*/
int Graph::solve() {
    int par = DUMMY_PARENT; 
    for (int i=0; i < n*m; i++){ dfs(i, par); next(); }
    return n*m - winnable.size();
}

/*  Print helper  */
void Graph::printSets() {
    for (auto winnab: winnable) debug(winnab);
    for (auto doomd: doomed) debug(doomd);
}
