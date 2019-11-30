#include <vector>
#include <iostream>
#include <map>
using namespace std;
#ifndef BOOTSTRAPPING
#define BOOTSTRAPPING
typedef vector<double> Vector; 
typedef vector<vector<double>> Matrix;

class Bootstrapping {
public:
	int num_group;
	vector<vector<string>>groups;
	void Sampling_Idx(vector<int>& return_vec, int idx_max, int k) const;
	void Sampling_Name(vector<string>& return_vec, vector<string>group) const;
	map<string, pair<double, double>>bench_mark;
	void Calculate(Matrix& return_mat) const;
	double stdev(Vector& series) const;
	void Download_stock(vector<pair<string, string>>);
};
#endif
