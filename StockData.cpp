#include "StockData.h"

int Download_data(stock* newstock, string name, string startTime, string endTime, struct MemoryStruct data, const char outfilename[FILENAME_MAX], CURL* handle)
{
	FILE* fp;
	CURLcode result;
	string sCookies, sCrumb;
	if (sCookies.length() == 0 || sCrumb.length() == 0)
	{
		fp = fopen(outfilename, "w");
		//curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
		curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
		curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

		curl_easy_setopt(handle, CURLOPT_ENCODING, "");
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
		result = curl_easy_perform(handle);
		fclose(fp);

		if (result != CURLE_OK)
		{
			// if errors have occurred, tell us what is wrong with result
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

		// perform, then store the expected code in result
		result = curl_easy_perform(handle);

		if (result != CURLE_OK)
		{
			// if errors have occured, tell us what is wrong with result
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}

		char cKey[] = "CrumbStore\":{\"crumb\":\"";
		char* ptr1 = strstr(data.memory, cKey);
		char* ptr2 = ptr1 + strlen(cKey);
		char* ptr3 = strstr(ptr2, "\"}");
		if (ptr3 != NULL)
			*ptr3 = NULL;

		sCrumb = ptr2;

		fp = fopen("cookies.txt", "r");
		char cCookies[100];
		if (fp) {
			while (fscanf(fp, "%s", cCookies) != EOF);
			fclose(fp);
		}
		else
			cerr << "cookies.txt does not exists" << endl;

		sCookies = cCookies;
		free(data.memory);
		data.memory = NULL;
		data.size = 0;
	}

	/*if (itr == stock_list.end())
	{
		break;
	}*/

	string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
	string symbol = name;
	string urlB = "?period1=";
	string urlC = "&period2=";
	string urlD = "&interval=1d&events=history&crumb=";
	string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
	const char* cURL = url.c_str();
	const char* cookies = sCookies.c_str();
	curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
	curl_easy_setopt(handle, CURLOPT_URL, cURL);

	/*
	fp = fopen(resultfilename, "ab");
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
	result = curl_easy_perform(handle);
	fclose(fp);

	// Check for errors
	if (result != CURLE_OK)
	{
		// if errors have occurred, tell us what is wrong with 'result'
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
		return 1;
	}
*/
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
	result = curl_easy_perform(handle);
	if (result != CURLE_OK)
	{
		// if errors have occurred, tell us what is wrong with result
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
		return 1;
	}

	//stringstream sData;
	//sData.str(data.memory);
	//string line;
	//getline(sData, line);
	//cout << line << endl;
	//for (; getline(sData, line); )
	//	cout << line << endl;

	/*stock* newstock = new stock((*itr).first, "2019-02-01");*/
	stringstream sData;
	sData.str(data.memory);
	string sValue, sDate;
	double dValue = 0;
	string line;
	getline(sData, line);


	while (getline(sData, line)) {
		//cout << line << endl;
		sDate = line.substr(0, line.find_first_of(','));
		line.erase(line.find_last_of(','));
		sValue = line.substr(line.find_last_of(',') + 1);
		dValue = strtod(sValue.c_str(), NULL);
		newstock->adjustedprice.push_back(dValue);
		newstock->alltime.push_back(sDate);
		newstock->abnormal_return.push_back(NAN);
		//   cout << sDate << " " << std::fixed << ::setprecision(6) << dValue << endl;
	}
}

void stock::display()
{
	for (auto i = 0; i < alltime.size(); i++)
	{
		cout << alltime[i] << "\t" << adjustedprice[i] << "\t" << abnormal_return[i] << endl;
	}
	cout << "stock destructor called" << endl;
}
void StockData::display()
{
	for (auto itr = stock_map.begin(); itr != stock_map.end(); itr++)
	{
		cout << "stock name: " << (*itr).first << endl;
		(*itr).second->display();
	}
}
StockData::~StockData()
{
	for (auto itr = stock_map.begin(); itr != stock_map.end(); itr++)
	{
		delete (*itr).second;
		(*itr).second = NULL;
	}
}
int StockData::Download_stock(vector<pair<string, string>>& stock_list, map<string, stock*>& stock_map)
{
	const int redundant_day = 60;//redundant day before/after pivot
	string startTime;
	string endTime;
	auto itr = stock_list.begin();

	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	// file pointer to create file that store the data  
	//FILE* fp;

	// name of files  
	const char outfilename[FILENAME_MAX] = "Output.txt";
	const char resultfilename[FILENAME_MAX] = "Results.txt";

	// declaration of an object CURL 
	CURL* handle;

	//CURLcode result;

	// set up the program environment that libcurl needs 
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();

	// if everything's all right with the easy handle... 
	if (handle)
	{
		while (itr != stock_list.end())
		{
			const string date_suffix = "T16:00:00";
			stock* newstock = new stock((*itr).first, (*itr).second + date_suffix);
			startTime = to_string(stoi(getTimeinSeconds((*itr).second + date_suffix)) - redundant_day * 86400);
			endTime = to_string(stoi(getTimeinSeconds((*itr).second + date_suffix)) + redundant_day * 86400);
			if (Download_data(newstock, (*itr).first, startTime, endTime, data, outfilename, handle) != 1)
			{
				stock_map.insert(make_pair((*itr).first, newstock));
				itr++;
			}
			else { return 0; }
		}
		free(data.memory);
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}
}

void StockData::Multi_thread_Download_stock(vector<pair<string, string>>& stock_list)
{
	const int num_list = 8;//thread number
	vector< map<string, stock*>>temp_map_vec(num_list);
	vector< vector<pair<string, string>>> temp_stock_list_vec(num_list);
	for (int i = 0; i < num_list - 1; i++)//divide stock_list into 4 temp_list
	{
		for (int j = (int)((double)i * (double)stock_list.size() / (double)num_list); j < (int)(((double)i + 1) * (double)stock_list.size() / (double)num_list); j++)
		{
			temp_stock_list_vec[i].push_back(stock_list[j]);
		}
	}
	for (int j = (int)(((double)num_list - 1) * (double)stock_list.size() / (double)num_list); j < stock_list.size(); j++)// fill the rest of vec into n-1(last) row
	{
		temp_stock_list_vec[num_list - 1].push_back(stock_list[j]);
	}

	vector<thread> threads;
	for (int i = 0; i < num_list; i++)
	{
		threads.push_back(thread(Download_stock, ref(temp_stock_list_vec[i]), ref(temp_map_vec[i])));
	}
	for (int i = 0; i < num_list; i++)
	{
		threads[i].join();
	}

	stock_map.clear();
	for (auto itr = temp_map_vec.begin(); itr != temp_map_vec.end(); itr++)
	{
		for (auto itr2 = (*itr).begin(); itr2 != (*itr).end(); itr2++)
		{
			stock_map.insert(*itr2);
		}
	}
}

void StockData::filter(map<string, stock*>& stock_map, vector<pair<string, string>>& stock_list)
{
	for (auto itr = stock_map.begin(); itr != stock_map.end(); itr++)
	{
		string date = (*itr).second->getdate();
		vector<string>::iterator ite1 = find((*itr).second->alltime.begin(), (*itr).second->alltime.end(), date);
		auto index = distance(begin((*itr).second->alltime), ite1);
		if ((index < 30) || ((*itr).second->alltime.size() - index) < 30)
		{
			stock_map.erase(itr);
			string name = itr->second->getname();
			for (auto itr2 = stock_list.begin(); itr2 != stock_list.end(); itr2++)
			{
				if (name == itr2->first)
					stock_list.erase(itr2);
			}
		}

	}
}