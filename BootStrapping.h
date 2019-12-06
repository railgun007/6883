#ifndef BootStrapping_h
#define BootStrapping_h
#include "WriteData.h"
#include "StockData.h"
#include "Matrix.h"
#include <locale>
#include <tuple>
#include <algorithm>

using namespace std;
//typedef vector<double> Vector;
//typedef vector<Vector> Matrix;
//typedef vector<Matrix> HMatrix;

class BootStrapping
{
public:
	int num_group = 3;
	map<string, pair<double, double>>benchmark;
	vector<vector<pair<string, string>>>groups;
	vector<pair<string, string>> stock_list;
	BootStrapping(vector<pair<string, string>> vec) :stock_list(vec) { Divide_vector(vec); };
	template<typename T>
	void Sampling_name(vector<const T*>& return_vec, const vector<T>& group, int sampling_num) const;
	//void Sampling_name(vector<const pair<string, string>*>& return_vec, const vector<pair<string, string>>& group, int sampling_num) const;
	void set_benchmark(StockData& data_container, string bench_mark_name = "SPY");
	void print_benchmark() const;
	void Calculate(HMatrix& return_mat, StockData& data_container, int sampling_num, int sampling_time);
	double Stdev(Vector& series) const;
	double Mean(Vector& series) const;
	void Divide_vector(const vector<pair<string, string>>& vec);
};

template<typename T>
void BootStrapping::Sampling_name(vector<const T*>& return_vec, const vector<T>& group, int sampling_num) const //must define here, otherwise Link error
{
	return_vec.resize(sampling_num);
	if (group.size() < sampling_num) { return; }
	vector<int> vect;
	for (int i = 0; i < group.size(); i++) vect.push_back(i);
	random_shuffle(vect.begin(), vect.end());
	for (int i = 0; i < return_vec.size(); i++)
	{
		return_vec[i] = &group[vect[i]];
	}
}
#endif // !BootStrapping_h