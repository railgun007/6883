#ifndef StockData_h
#define StockData_h
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "WriteData.h"
using namespace std;

class stock
{
private:
	string time;
	string name;
public:
	string getname() { return name; }
	string getdate() { return time; }
	stock(string name_, string time_) { time = time_; name = name_; }
	vector<double> adjustedprice;
	vector<string> alltime;
	vector<double> abnormal_return;
	void display();
};
class StockData
{
public:
	map<string, stock*> stock_map;
	int Download_stock(vector<pair<string, string>>& stock_list);
	void display();
	~StockData();
};
int Download_data(stock* newstock, string name, string startTime, string endTime, struct MemoryStruct data, const char outfilename[FILENAME_MAX], CURL* handle);
#endif // !StockData_h