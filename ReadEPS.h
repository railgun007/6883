#ifndef ReadEPS_h
#define ReadEPS_h
#include <string>
#include <tuple>
#include <algorithm>
#include "Matrix.h"

using namespace std;
class ReadEPS
{
public:
	vector<string> stock_name;
	vector<string> release_date;
	Vector eps;
	Vector estimated_eps;
	Vector suprise;
	ReadEPS() {};
	ReadEPS(string csv_address) { Read_csv(csv_address); }
	void Read_csv(string csv_address);
	vector<pair<string, string>> sort_by_suprise();
};
#endif // !ReadEPS_h