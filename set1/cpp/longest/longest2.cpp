#include <iostream>
#include <vector> 
#include <fstream>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;
 
// Exei xrisimopoiithei kodikas apo to link toy geeksforgeeks
// https://www.geeksforgeeks.org/longest-subarray-having-average-greater-than-or-equal-to-x-set-2/
int maxIndexDiff(vector<float> arr, int n)
{
    int maxDiff = -1;
    int i, j;
 
    float LMin[n], RMax[n];
 
    LMin[0] = arr[0];
    for (i = 1; i < n; ++i)
        LMin[i] = min(arr[i], LMin[i - 1]);
    RMax[n - 1] = arr[n - 1];
    for (j = n - 2; j >= 0; --j)
        RMax[j] = max(arr[j], RMax[j + 1]);

    i = 0, j = 0;
    while (j < n && i < n) {
        if (LMin[i] < RMax[j]) {
            maxDiff = max(maxDiff, j - i);
            j = j + 1;
        }
        else
            i = i + 1;
    }
    if (maxDiff == n-1) return maxDiff + 1;
    else return maxDiff;
}
 
vector<float> modifyarr(vector<float> arr, int n, int x)
{
    for (int i = 0; i < n; i++)
        arr[i] = arr[i] - x;

    return arr;
}
 
vector<float> calcprefix(vector<float> arr, int n)
{
    float s = 0.0;
    for (int i = 0; i < n; i++) {
        s += arr[i];
        arr[i] = s;
    }
    return arr;
}
 
int longestsubarray(vector<float> arr, int n, int x)
{
    return maxIndexDiff(calcprefix(modifyarr(arr, n, x), n), n);
}
 
int main(int argc, char** argv){
    ios_base::sync_with_stdio(false);
	fstream file;
	string filename = argv[1];
	file.open(filename.c_str());
	
	string current;
	int days;
	file >> current;
	days = stoi(current);
	int hospitals;
	file >> current;
	hospitals = stoi(current);

    vector<float> temp;
	for (int j=0; j<days; j++){
        file >> current ;
		temp.push_back(-1.0*stoi(current)/hospitals);
    }
    int x = 1;
    // printVector(temp);
    int res = longestsubarray(temp, days, x);
    cout << res << endl;
 
    return 0;
}