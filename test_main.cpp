#include "StockData.h"
#include "BootStrapping.h"
#include "ReadEPS.h"

int main()
{
	ReadEPS read_obj("Bloomberg.csv");
	cout << read_obj.suprise << endl;
	vector<pair<string, string>> temp;
	read_obj.sort_by_suprise(temp);
	for_each(temp.begin(), temp.end(), [](auto ele) {cout << ele.first << "\t" << ele.second << endl; });
	/*vector<pair<string, string>> stock_list;
	stock_list.push_back(make_pair("AMZN", "2019-04-01"));
	stock_list.push_back(make_pair("MSFT", "2019-04-01"));
	stock_list.push_back(make_pair("TWTR", "2019-04-01"));
	StockData data_container;
	data_container.Download_stock(stock_list);
	data_container.display();*/

	/*BootStrapping obj(stock_list);
	obj.set_benchmark(data_container);
	obj.print_benchmark();*/

	/*vector<pair<string, string>> vec_test(10, make_pair(to_string(0), to_string(0)));
	for (int i = 0; i < vec_test.size(); i++)
	{
		vec_test[i] = make_pair(to_string(i), to_string(i));
	}
	vector<const pair<string, string>*>return_vec;
	obj.Sampling_name<pair<string, string>>(return_vec, vec_test, 5);
	for_each(vec_test.begin(), vec_test.end(), [](auto ele) {cout << ele.first << endl; });
	cout << endl;
	for_each(return_vec.begin(), return_vec.end(), [](auto ele) {cout << (*ele).first << endl; });*/

	/*HMatrix res_mat;
	obj.Calculate(res_mat, data_container, 1, 2);
	data_container.display();
	cout << res_mat << endl;*/

	system("pause");
	return 0;
}