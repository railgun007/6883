#include "stock.h"


void stock::display()
{
	for (auto i = 0; i < alltime.size(); i++)
	{
		cout << alltime[i] << "\t" << adjustedprice[i] << endl;
	}
}