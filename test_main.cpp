#include "StockData.h"
#include "BootStrapping.h"

int main()
{
	vector<pair<string, string>> stock_list;
	stock_list.push_back(make_pair("AMZN", "2019-04-01T16:00:00"));
	stock_list.push_back(make_pair("MSFT", "2019-04-01T16:00:00"));
	stock_list.push_back(make_pair("TWTR", "2019-04-01T16:00:00"));
	StockData data_container;
	data_container.Download_stock(stock_list);
	data_container.display();

	BootStrapping obj(stock_list);
	obj.set_benchmark(data_container);
	obj.print_benchmark();
	vector<string> vec_test(10, "a");
	for (int i = 0; i < vec_test.size(); i++)
	{
		vec_test[i] = to_string(i);
	}
	vector<string*>return_vec;
	obj.Sampling_name(return_vec, vec_test, 5);
	for_each(vec_test.begin(), vec_test.end(), [](auto ele) {cout << ele << endl; });
	cout << endl;
	for_each(return_vec.begin(), return_vec.end(), [](auto ele) {cout << (*ele) << endl; });

	system("pause");
	return 0;
}