#ifndef ReadEPS_h
#define ReadEPS_h
#include <string>
#include <tuple>
#include <algorithm>
#include "Matrix.h"
#include<map>
using namespace std;
class ReadEPS
{
public:
	vector<string> stock_name;
	vector<string> release_date;
	Vector eps;
	Vector estimated_eps;
	Vector suprise;
	map<string, tuple<double, double, string, double>>  info_map; //it contaions actual eps, estimate eps,release date and surprice
	ReadEPS(string csv_address) //put the necessary information into a tuple
	{
		Read_csv(csv_address);
		suprise = (eps - estimated_eps) / estimated_eps;
		for (int i = 0; i < stock_name.size(); i++) info_map[stock_name[i]] = make_tuple(eps[i], estimated_eps[i], release_date[i], suprise[i]);
	}
	ReadEPS() {};

	void Read_csv(string csv_address);//read the bloomberg csv file
	void sort_by_suprise(vector<pair<string, string>>& out_vec);//sort the stock by their surprise
};
#endif // !ReadEPS_h