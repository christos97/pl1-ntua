#include <iostream>
#include <vector> 
#include <fstream>
#include <string>
using namespace std;

void printVector(vector<int> path){
	for (auto i : path)
		cout << i << " ";
	cout << endl;
}

vector<int> checkAvg(int start, vector<int> table, int hospitals){
	int sum = 0;
	int curK = 0;
	int curAvg = 0;
	vector <int> curLong, longest;
	for (unsigned int i=start; i<table.size(); i++){
		curAvg = -1 * (sum + table[i]) / ((curK + 1) * hospitals);
		curK++;
		sum += table[i];
		curLong.push_back(table[i]);
		if (curAvg >= 1)
			longest = curLong;
	}
	cout << "From: " << start << " Vector: ";
	printVector(longest);
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
	vector<int> temp, tempMax, longest;
	size_t max = 0;
	size_t count = 0;
	while (file >> current){
		temp.push_back(stoi(current));
	}
	
	while (count < temp.size()){
		vector<int> curLong = checkAvg(count++, temp, hospitals);
		if (curLong.size() > max){
			max = curLong.size();
			longest.swap(curLong);
		}
	}
	printVector(longest);
	return 0;
}