#ifndef Menu_h
#define Menu_h
#include "StockData.h"
#include "BootStrapping.h"
#include "ReadEPS.h"
#include "DrawPlotandShow.h"
using namespace std;

void Menu(vector<pair<string, string>>& stock_list, StockData& data_container, ReadEPS& read_obj, HMatrix& res_mat, bool& WantToExit, bool& DataIsRetrived);
#endif // !Menu_h