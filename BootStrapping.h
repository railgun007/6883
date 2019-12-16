#ifndef BootStrapping_h
#define BootStrapping_h
#include "WriteData.h"
#include "StockData.h"
#include "Matrix.h"
#include <locale>
#include <tuple>
#include <algorithm>
#include <time.h>

using namespace std;
class BootStrapping
{
public:
	int num_group = 3;
	map<string, pair<double, double>>benchmark;//string is the date  the double pair is price and return
	vector<vector<pair<string, string>>>groups; //the string pair is the name and date
	vector<pair<string, string>> stock_list;   //a vector of stock name and release date
	BootStrapping(vector<pair<string, string>> vec) :stock_list(vec) { Divide_vector(vec); };//divide stock into 3 groups when calling constructor
	template<typename T>
	void Sampling_name(vector<const T*>& return_vec, const vector<T>& group, int sampling_num) const;//choose 30 stocks from one group
	void set_benchmark(StockData& data_container, string bench_mark_name = "SPY");//read the benchmark data and save it in a container
	void print_benchmark() const;//print the value of benchmark and its date
	void Calculate(HMatrix& return_mat, StockData& data_container, int sampling_num, int sampling_time);//calcluate AAR CAAR AAR-SD CAAR-SD
	double Stdev(Vector& series) const;//the method to calculate standard deviation of value
	double Mean(Vector& series) const;//the method to calculate mean of value
	void Divide_vector(const vector<pair<string, string>>& vec);//divide stock into 3 groups 
};

template<typename T>
void BootStrapping::Sampling_name(vector<const T*>& return_vec, const vector<T>& group, int sampling_num) const // define here, otherwise Link error
{
	srand(clock());
	vector<int> vect;
	for (int i = 0; i < group.size(); i++) vect.push_back(i);
	random_shuffle(vect.begin(), vect.end());//change the location of all value in this vector
	for (int i = 0; i < sampling_num; i++)  return_vec.push_back(&group[vect[i]]);
}
#endif // !BootStrapping_h