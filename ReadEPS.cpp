#include "ReadEPS.h"
#include <fstream>
#include <sstream>

void Read_csv(string csv_address) {
	vector<string> matrix;
	//readfile
	fstream file;
	file.open(csv_address);
	string line;
	while (getline(file, line, '\n'))
	{
		istringstream templine(line);
		string data;
		while (getline(templine, data, ','))
		{
			matrix.push_back(data);
		}
	}
	file.close();
	vector<string>ticker;
	vector<string>date;
	vector<double>Act;
	vector<double>Est;
	for (int i = 4; i < matrix.size(); i = i + 4)
	{
		ticker.push_back(matrix[i]);
		date.push_back(matrix[i + 1]);
		Act.push_back(stod(matrix[i + 2]));
		Est.push_back(stod(matrix[i + 3]));
	}
	stock_name = ticker;
	release_date = date;
	eps = Act;
	estimated_eps = Est;
}


vector<pair<string, string>> ReadEPS::sort_by_suprise()
{

	vector<tuple<string, string, double>>temp;
	for (int i = 0; i < stock_name.size(); i++)
	{
		temp.push_back(make_tuple(stock_name[i], release_date[i], suprise[i]));
	}

	sort(temp.begin(), temp.end(), [](auto x, auto y) {get<2>(x) < get<2>(y); });
	vector<pair<string, string>>out_vec;
	for_each(temp.begin(), temp.end(), [&](auto ele) {out_vec.push_back(make_pair(get<0>(ele), get<1>(ele)); });
	return out_vec;
}
