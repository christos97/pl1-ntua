#include <iostream>
#include <vector> 
#include <fstream>
#include <time.h>
using namespace std;
const clock_t begin_time = clock();

void printVector(vector<int> path){
	for (auto i : path)
		cout << i << " ";
	cout << endl;
}

int checkAvg(int start, vector<int> table, int hospitals, int max){
	int sum = 0;
	int curK = 0;
	int curAvg = 0;
	int longest = 0;  
	for (unsigned int i=start; i<table.size(); i++){
		curAvg = -1 * (sum + table[i]) / ((curK + 1) * hospitals);
		curK++;
		sum += table[i];
		if (curAvg >= 1)
			longest = curK;
	}
	// cout << "From: " << start << " Vector: ";
	// printVector(longest);
	return longest;
}

int main(int argc, char** argv) {
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
	cout << days << " " << hospitals << endl;
	
	vector<int> temp;
	int max = 0;
	for (int j=0; j<days; j++){
		//curMax = checkAvg(count++, temp, hospitals, max);
		int sum = 0;
		int curK = 0;
		int curAvg = 0;
		int curMax = 0;
		for (int i=j; i<days; i++){
			if (j == 0){
				file >> current ;
				temp.push_back(stoi(current));
			}
			curAvg = -1 * (sum + temp[i]) / ((curK + 1) * hospitals);
			curK++;
			sum += temp[i];
			if (curAvg >= 1)
				curMax = curK;
		}
		if (curMax > max){
			max = curMax;
		}
		if ((days-max <= j)) break;
	}
	cout << max << endl;
	cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
	return 0;
}