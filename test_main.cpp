#include "StockData.h"
#include "BootStrapping.h"
#include "ReadEPS.h"
#include "DrawPlotandShow.h"
#include "Menu.h"
using namespace std;

int main()
{

	/*cout << read_obj.suprise << endl;*/
	vector<pair<string, string>> stock_list;

	/*for_each(stock_list.begin(), stock_list.end(), [](auto ele) {cout << ele.first << "\t" << ele.second << endl; });*/

	StockData data_container;
	ReadEPS read_obj("Bloomberg.csv");
	const string gnuplot_address = "D:/Software/gnuplot/bin/gnuplot.exe";
	HMatrix res_mat;

	bool WantToExit = false;
	bool DataIsRetrived = false;

	do 
	{
		Menu(stock_list, data_container, read_obj, res_mat, gnuplot_address, WantToExit, DataIsRetrived);
	} while (!WantToExit);

	system("pause");
	return 0;
}