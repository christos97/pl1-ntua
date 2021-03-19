#include<bits/stdc++.h>
using namespace std;
ifstream file;
vector<vector<char>> maze;
int main(int argc, char **argv) {
    string out;
    file.open(argv[1]);
    file >> out; int n = stoi(out);
    file >> out; int m = stoi(out);
    maze.resize(n*m);
    int row = 0;
    while (file >> out) for (char& c : out) { maze[row].push_back(c); row++; }
    
    for (auto& i : maze) {
        for (auto& j : i) {
            i.
            cout << j << endl;
        }
    }

}