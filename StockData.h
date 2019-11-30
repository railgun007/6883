#include "stock.h"
#include <map>
#ifndef STOCKDATA_h
#define STOCKDATA_h
class StockData
{
public:
	map<string, stock*> stock_map;
	void display();
}; 
#endif