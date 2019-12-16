#include "Menu.h"

void Menu(vector<pair<string, string>>& stock_list, StockData& data_container, ReadEPS& read_obj, HMatrix& res_mat, const string& gnuplot_address, bool& WantToExit, bool& DataIsRetrived)
{
	int Option;
	cout << "====================== MENU of OPTIONS ======================" << endl << endl;
	cout << "1. Retrieve historical price data for all stocks" << endl;
	cout << "2. Pull information for one stock from one group" << endl;
	cout << "3. Show AAR or AAR-SD or CAAR or CAAR-SD for one Group" << endl;
	cout << "4. Show the Gnuplot graph with CAAR for all 3 groups" << endl;
	cout << "5. Exit your program" << endl << endl;
	cout << "Please enter a number from 1 to 5: ";

	cin >> Option;

	if (cin.fail())
	{
		cout << endl;
		cout << "# Please enter a VALID number (from 1 to 5). #" << endl << endl;
		cin.clear(); cin.ignore(100, '\n');
		return;
	}

	switch (Option)
	{
	case 1:
	{
		if (DataIsRetrived)
		{
			cout << endl;
			cout << "# Historical price data has already been retrieved. #" << endl << endl;
			break;
		}
		else {
			cout << "    ";
			read_obj.sort_by_suprise(stock_list);
			data_container.Multi_thread_Download_stock(stock_list);
			data_container.filter(data_container.stock_map, stock_list);//delete those stocks lack of data
			BootStrapping obj(stock_list);
			obj.set_benchmark(data_container);
			obj.print_benchmark();
			cout << endl << endl;
			cout << "# Historical Data Retrieve Done. #" << endl << endl;
			obj.Calculate(res_mat, data_container, 30, 30);
			DataIsRetrived = true;
			break;
		}
	}
	case 2:
	{
		cout << endl;
		string stockChoice;
		if (!DataIsRetrived)
		{
			cout << "# You have to retrieve historical data FIRST. Please enter 1. #" << endl << endl;
			break;
		}
		else {
			cout << "Please enter a stock ticker: ";
			cin >> stockChoice;

			while (data_container.stock_map.count(stockChoice) == 0)
			{
				cout << endl;
				cout << "# Stock not found. #" << endl << endl;
				cout << "Please pick a stock ticker from following tickers: " << endl;
				for (auto itr = data_container.stock_map.begin(); itr != data_container.stock_map.end(); itr++) cout << itr->first << endl;
				cout << "Please enter a stock ticker: ";
				cin >> stockChoice;
			}

			auto iter = data_container.stock_map.find(stockChoice);
			cout << "Please enter a stock ticker: ";
			cout << endl;
			cout << "-----------------------------------------------------------" << endl;
			cout << "Below is the Stock Info:" << endl << endl;
			cout << "Ticker:         " << stockChoice << endl;
			cout << "Group:          " << iter->second->group_number << endl;
			cout << "Actual EPS: " << get<0>(read_obj.info_map[stockChoice]) << endl;
			cout << "EPS Estimate:   " << get<1>(read_obj.info_map[stockChoice]) << endl;
			cout << "Surprise:       " << get<3>(read_obj.info_map[stockChoice]) << endl;
			cout << "Release Date " << get<2>(read_obj.info_map[stockChoice]) << endl;
			cout << "Start of 61 Trading Days: " << iter->second->alltime[iter->second->start_index] << endl;
			cout << "End of 61 Trading Days:   " << iter->second->alltime[iter->second->end_index] << endl;
			cout << "price of 61 days" << endl;
			iter->second->display();//show the price and return of one stock
			break;

		}
	}


	case 3:
	{
		cout << endl;
		if (!DataIsRetrived)
		{
			cout << "# You have to retrieve historical data FIRST. Please enter 1. #" << endl << endl;
			break;
		}
		else {
			int group;
			int type;

			cout << "Please choice a group (1 for Miss, 2 for Meet, 3 for Beat): ";
			cin >> group;
			while (((group != 1) && (group != 2) && (group != 3)) || (cin.fail()))
			{
				cout << endl;
				cout << "# Please enter a VALID number. #" << endl << endl;
				cout << "Please choice a group (1 for Miss, 2 for Meet, 3 for Beat): ";
				cin.clear(); cin.ignore(100, '\n');
				cin >> group;
			}

			cout << endl;
			cout << "Please choice AAR or AAR-SD or CAAR or CAAR-SD (1 for AAR, 2 for AAR-SD, 3 for CAAR, 4 for CAAR-SD ): ";
			cin >> type;
			while (((type != 1) && (type != 2) && (type != 3) && (type != 4)) && (cin.fail()))
			{
				cout << endl;
				cout << "# Please enter a VALID number. #" << endl << endl;
				cout << "Please choice AAR or AAR-SD or CAAR or CAAR-SD (1 for AAR, 2 for AAR-SD, 3 for CAAR, 4 for CAAR-SD ): ";
				cin.clear(); cin.ignore(100, '\n');
				cin >> type;
			}
			Choose_to_show(group, type, res_mat);



			break;
		}
	}


	case 4:
	{
		if (!DataIsRetrived)
		{
			cout << endl;
			cout << "# You have to retrieve historical data FIRST. Please enter 1. #" << endl << endl;
			break;
		}
		else {
			Plot_CAAR(res_mat, gnuplot_address);
			cout << endl;
			break;
		}
	}


	case 5:
	{
		WantToExit = true;
		break;
	}


	default:
	{
		cout << endl;
		cout << "# Please enter a VALID number (from 1 to 5). #" << endl << endl;
		break;
	}
	}

	cout << endl;
	cout << "# Thank You! #" << endl << endl;
}