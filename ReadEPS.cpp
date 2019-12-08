#include "ReadEPS.h"

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