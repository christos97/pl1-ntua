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
set<int> visited;
enum Direction { UP = 28, DOWN = 11, RIGHT = 25, LEFT = 19, OUT_OF_BOUNDS = -1, BLACK_LIST = -404 };
int n, m;
bool existsInWinnable(int& next);
bool existsInVisited(int& next);


int main(int argc, char **argv) {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    file.open(argv[1]);
    file >> out; n = stoi(out);
    file >> out; m = stoi(out);
    int graph[n*m];
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
                            graph[vertex] =  OUT_OF_BOUNDS;
                            winnable.insert(vertex);
                        }
                        else graph[vertex] =  vertex - m;
                        break;
                    }
                    case DOWN: {
                        if (row == n-1) {
                            graph[vertex] =  OUT_OF_BOUNDS;
                            winnable.insert(vertex);
                        }
                        else graph[vertex] =  vertex + m;
                        break;
                    }
                    case LEFT: {
                        if (col == 0) {
                            graph[vertex] =  OUT_OF_BOUNDS;
                            winnable.insert(vertex);
                        }
                        else graph[vertex] =  vertex - 1;
                        break;
                    }
                    case RIGHT: {
                        if (col == m-1) {
                            graph[vertex] =  OUT_OF_BOUNDS;
                            winnable.insert(vertex);
                        }
                        else graph[vertex] =  vertex + 1;
                        break;
                    }
                    default: perror("invalid char"); exit(1);
                }
                vertex++;
            }
            else {
                switch (direction){
                    case UP: graph[vertex] =  vertex - m; break;
                    case DOWN: graph[vertex] =  vertex + m; break;
                    case LEFT: graph[vertex] =  vertex - 1; break;
                    case RIGHT: graph[vertex] =  vertex + 1; break;
                    default: perror("invalid char"); exit(1);
                }
                vertex++;
            }
        }
    } 
    file.close();
    
    for (int i=0; i < n*m; i++){
        int j = i;
        visited.clear();
        while (true) {
            visited.insert(j);
            if (graph[j] == BLACK_LIST) {
                for (auto room : visited){
                    graph[room] = BLACK_LIST;
                }
                break;
            } else if (graph[j] == OUT_OF_BOUNDS){
                for (auto room : visited){
                    graph[room] = OUT_OF_BOUNDS;
                    if (!existsInWinnable(room)) winnable.insert(room);
                }
                break;
            } else {
                if (!existsInVisited(graph[j])){
                    j = graph[j];
                    continue;
                } else {
                    for (auto room : visited){
                        graph[room] = BLACK_LIST;
                    }
                    break;
                }
            }
        }
    }
    int doomed = n*m - winnable.size();
    cout << doomed << endl;
}

inline bool existsInWinnable(int& next){
    return winnable.find(next) != winnable.end();
}

inline bool existsInVisited(int& next){
    return visited.find(next) != visited.end();
}