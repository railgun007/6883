#ifndef BootStrapping_h
#define BootStrapping_h
#include "WriteData.h"
#include "StockData.h"
#include <locale>
#include <algorithm>

using namespace std;
typedef vector<double> Vector;
typedef vector<Vector> Matrix;
typedef vector<Matrix> HMatrix;

class BootStrapping
{
public:
	int num_group = 3;
	map<string, pair<double, double>>benchmark;
	vector<vector<string>>groups;
	vector<pair<string, string>> stock_list;
	BootStrapping(vector<pair<string, string>> vec) :stock_list(vec) { Divide_vector(vec); };
	void Sampling_name(vector<string*>& return_vec, vector<string>& group, int sampling_num) const;
	void set_benchmark(StockData& data_container, string bench_mark_name = "SPY");
	void print_benchmark() const;
	void Calculate(HMatrix& return_mat) const;
	double Stdev(Vector& series) const;
	void Divide_vector(const vector<pair<string, string>>& vec);
};
#endif // !BootStrapping_h