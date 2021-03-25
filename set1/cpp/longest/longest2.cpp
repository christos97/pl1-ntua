#include <iostream>
#include <chrono>
#include <vector> 
#include <fstream>
#include <string>
using namespace std;
int days; /* Grid rows */ 
int hospitals; /* Grid cols */
int main(int argc, char **argv) {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    auto start = chrono::steady_clock::now();
    
    ifstream file;
    string out;
    file.open(argv[1]);
    file >> out; days = stoi(out);
    file >> out; hospitals = stoi(out);




    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
}