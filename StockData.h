#ifndef StockData_h
#define StockData_h
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
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
	int start_index;   //we use start index and end index to get 60 days of data
	int end_index;
	string group_number;
	vector<double> adjustedprice;
	vector<string> alltime;
	vector<double> abnormal_return;
	void display();//display the price and return of a stock
};
class StockData
{
public:
	map<string, stock*> stock_map;//string is the stock name, and stock is the object contains the information of one stock
	static int Download_stock(vector<pair<string, string>>& stock_list, map<string, stock*>& stock_map);//this is the function invoked by multi thread funtion
	void Multi_thread_Download_stock(vector<pair<string, string>>& stock_list);//use multi-thread to download data
	void filter(map<string, stock*>& stock_map, vector<pair<string, string>>& stock_list);//we filter the stock with data less than we want
	~StockData();//destructor
};
int Download_data(stock* newstock, string name, string startTime, string endTime, struct MemoryStruct& data, const char outfilename[FILENAME_MAX], CURL* handle, FILE* fp, CURLcode& result, string& sCookies, string& sCrumb);
#endif // !StockData_h