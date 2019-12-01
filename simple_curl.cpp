#include <stdio.h>
#include <string> 
#include <iostream>
#include <sstream>  
#include <vector>
#include <locale>
#include <iomanip>
#include "curl.h"
#include <map>
#include <algorithm>

using namespace std;

int write_data(void* ptr, int size, int nmemb, FILE* stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}
struct MemoryStruct {
	char* memory;
	size_t size;
};
void* myrealloc(void* ptr, size_t size)
{
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)data;
	mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string getTimeinSeconds(string Time)
{
	std::tm t = { 0 };
	std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
	if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
	{
		cout << std::put_time(&t, "%c %Z") << "\n"
			<< std::mktime(&t) << "\n";
		sprintf(time, "%lld", mktime(&t));
		return string(time);
	}
	else
	{
		cout << "Parse failed\n";
		return "";
	}
}

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
void stock::display()
{
	for (auto i = 0; i < alltime.size(); i++)
	{
		cout << alltime[i] << "\t" << adjustedprice[i] << endl;
	}
	cout << "stock destructor called" << endl;
}

class StockData
{
public:
	map<string, stock*> stock_map;
	void display();
	~StockData()
	{
		for (auto itr = stock_map.begin(); itr != stock_map.end(); itr++)
		{
			delete (*itr).second;
			(*itr).second = NULL;
		}
	}
};
void StockData::display()
{
	for (auto itr = stock_map.begin(); itr != stock_map.end(); itr++)
	{
		cout << "stock name: " << (*itr).first << endl;
		(*itr).second->display();
	}
}
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
		//   cout << sDate << " " << std::fixed << ::setprecision(6) << dValue << endl;
	}
}
int Download_stock(StockData& data_container, vector<pair<string, string>> stock_list)
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
		while (itr!= stock_list.end())
		{
			stock* newstock = new stock((*itr).first, (*itr).second);
			startTime = to_string(stoi(getTimeinSeconds((*itr).second)) - redundant_day * 86400);
			endTime = to_string(stoi(getTimeinSeconds((*itr).second)) + redundant_day * 86400);
			if (Download_data(newstock, (*itr).first, startTime, endTime,data, outfilename, handle) != 1)
			{
				data_container.stock_map.insert(make_pair((*itr).first, newstock));
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

void set_bench_mark(StockData& data_container, map<string, pair<double, double>>& return_map, string bench_mark_name = "SPY")
{
	int smallest_time_sec = INT32_MAX, largest_time_sec = 0;
	for (auto itr = data_container.stock_map.begin(); itr != data_container.stock_map.end(); itr++)//find out the start/end time for benchmark
	{
		if (stoi(getTimeinSeconds((*itr).second->alltime.back() + "T16:00:00")) > largest_time_sec)
		{
			largest_time_sec = stoi(getTimeinSeconds((*itr).second->alltime.back() + "T16:00:00"));
		}
		if (stoi(getTimeinSeconds((*itr).second->alltime.front() + "T16:00:00")) < smallest_time_sec)
		{
			smallest_time_sec = stoi(getTimeinSeconds((*itr).second->alltime.front() + "T16:00:00"));
		}
	}
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
	if (handle)
	{
		stock* new_bench_mark = new stock(bench_mark_name, "");
		string startTime = to_string(smallest_time_sec);
		string endTime = to_string(largest_time_sec);
		Download_data(new_bench_mark, bench_mark_name, startTime, endTime, data, outfilename, handle);
		return_map.insert(pair<string, pair<double, double>>(new_bench_mark->alltime[0], pair<double, double>(new_bench_mark->adjustedprice[0], NAN)));
		for (int i = 1; i < new_bench_mark->adjustedprice.size(); i++)
		{
			return_map.insert(pair<string, pair<double, double>>(new_bench_mark->alltime[i], pair<double, double>(new_bench_mark->adjustedprice[i],
				(new_bench_mark->adjustedprice[i] - new_bench_mark->adjustedprice[i - 1]) / new_bench_mark->adjustedprice[i - 1])));
		}
		delete new_bench_mark;
		new_bench_mark = NULL;
	}
}
int main(void)
{
	vector<pair<string, string>> stock_list;
	stock_list.push_back(make_pair("AMZN", "2019-04-01T16:00:00"));
	stock_list.push_back(make_pair("MSFT", "2019-04-01T16:00:00"));
	stock_list.push_back(make_pair("TWTR", "2019-04-01T16:00:00"));
	StockData data_container;
	Download_stock(data_container, stock_list);
	data_container.display();

	map<string, pair<double, double>> bench_mark;
	set_bench_mark(data_container, bench_mark);
	for_each(bench_mark.begin(), bench_mark.end(), [](pair<string, pair<double, double>> ele) 
		{cout << ele.first << "\t" << ele.second.first << "\t" << ele.second.second << endl; });//cout the benchmark
	
	system("pause");
	return 0;
}