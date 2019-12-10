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

	map<string, tuple<double, double, string, double>>  info_map;
	ReadEPS(string csv_address)
	{
		Read_csv(csv_address);
		suprise = (eps - estimated_eps) / estimated_eps;
		for (int i = 0; i < stock_name.size(); i++) {
			info_map[stock_name[i]] = make_tuple(eps[i], estimated_eps[i], release_date[i], suprise[i]);
		}
	}
	ReadEPS() {};
	
	void Read_csv(string csv_address);
	void sort_by_suprise(vector<pair<string, string>>& out_vec);
};
#endif // !ReadEPS_h