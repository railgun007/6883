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
	data_container.Download_stock(stock_list);
	data_container.display();

	BootStrapping obj(stock_list);
	obj.set_benchmark(data_container);
	obj.print_benchmark();

	HMatrix res_mat;
	obj.Calculate(res_mat, data_container, 5, 5);
	/*data_container.display();
	cout << res_mat << endl;*/
	Choose_to_show(1, 1, res_mat);
	Plot_CAAR(res_mat);

	system("pause");
	return 0;
}