#include <vector>
#include <iostream>
#include <map>
using namespace std;
#ifndef BOOTSTRAPPING_h
#define BOOTSTRAPPING_h
typedef vector<double> Vector; 
typedef vector<vector<double>> Matrix;

class Bootstrapping {
public:
	int num_group;
	map<string, pair<double, double>>benchmark;
	vector<vector<string>>groups;
	void Sampling_Idx(vector<int>& return_vec, int idx_max, int k) const;
	void Sampling_Name(vector<string>& return_vec, vector<string>group) const;
	void set_benchmark() const;
	void Calculate(Matrix& return_mat) const;
	double stdev(Vector& series) const;
	void Download_stock(vector<pair<string, string>>);
};
#endif
