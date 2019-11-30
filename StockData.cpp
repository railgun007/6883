#include "StockData.h"
void StockData::display()
{
	for (auto itr = stock_map.begin(); itr != stock_map.end(); itr++)
	{
		cout << "stock name: " << (*itr).first << endl;
		(*itr).second->display();
	}
}