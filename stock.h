#include <string>
#include <iostream>
#include <vector>
using namespace std;
#ifndef STOCK_h
#define STOCK_h
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
	void display();
};
#endif // !STOCK

