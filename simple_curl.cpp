#include "StockData.h"
#include "BootStrapping.h"
#include "ReadEPS.h"
#include "DrawPlotandShow.h"

int main()
{
	ReadEPS read_obj("Bloomberg.csv");
	/*cout << read_obj.suprise << endl;*/
	vector<pair<string, string>> stock_list;
	read_obj.sort_by_suprise(stock_list);
	/*for_each(stock_list.begin(), stock_list.end(), [](auto ele) {cout << ele.first << "\t" << ele.second << endl; });*/

	StockData data_container;
	data_container.Multi_thread_Download_stock(stock_list);
	data_container.filter(data_container.stock_map, stock_list);
	//data_container.display();

	BootStrapping obj(stock_list);
	obj.set_benchmark(data_container);
	obj.print_benchmark();

	HMatrix res_mat;
	obj.Calculate(res_mat, data_container, 30, 30);
	//²âÊÔstart£¬endºÍgroup
	//map<string, stock*>::iterator iter;
	//iter = data_container.stock_map.find("AEP");
	//cout << "start" << iter->second->start_index << endl;

	//for (auto itr = data_container.stock_map.begin(); itr != data_container.stock_map.end(); itr++)
	//{
	//	cout << "start_index: " << (*itr).second->start_index << endl;
	//	cout << "end_index: " << (*itr).second->end_index << endl;
	//	cout << "group: " << (*itr).second->group_number << endl;
	//}
	///*data_container.display();*/
	cout << res_mat << endl;

	Choose_to_show(1, 1, res_mat);
	Plot_CAAR(res_mat);

	system("pause");
	return 0;
}