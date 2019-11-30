#include <stdio.h>
#include <string> 
#include <iostream>
#include <sstream>  
#include <vector>
#include <locale>
#include <iomanip>
#include "curl.h"
#include <map>
#include <time.h>

using namespace std;

int write_data(void *ptr, int size, int nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}
struct MemoryStruct {
	char *memory;
	size_t size;
};
void *myrealloc(void *ptr, size_t size)
{	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

int write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;
	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string getTimeinSeconds(string Time)
{
	std::tm t = {0};
    std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
    {
        cout << std::put_time(&t, "%c %Z") << "\n"
                  << std::mktime(&t) << "\n";
		sprintf (time, "%lld", mktime(&t));
		return string(time);
    }
    else
    {
        cout << "Parse failed\n";
		return "";
    }
}

class stock {//这就是那个object
private:
	string time;
	string name;
public:
	string getname() { return name; }
	string getdate() { return time; }
	stock(string name_, string time_) { time = time_; name = name_; }
	vector<double> adjustedprice;
	vector<string> alltime;

};
int main(void)
{
	clock_t start, end;
	float timep = 0;
	start = clock();
	typedef map<string, stock*> STOCKMAP;//总的map  放股票名字和pair
	typedef pair<string, stock*> STOCKPAIR; //pari  用来放release date和object
	STOCKMAP stockmap;
	string startTime = getTimeinSeconds("2019-01-01T16:00:00");   //这儿是爬虫的起始时间  要改的以后
	string endTime = getTimeinSeconds("2019-10-01T16:00:00"); //这儿是爬虫的终止时间  要改的以后
	vector<string> symbolList;
	symbolList.push_back(string("AMZN"));// 现在只弄了三只股票
	symbolList.push_back(string("MSFT"));
	symbolList.push_back(string("TWTR"));
	vector<string>::iterator itr = symbolList.begin();

	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	// file pointer to create file that store the data  
	FILE *fp;

	// name of files  
	const char outfilename[FILENAME_MAX] = "Output.txt";
	const char resultfilename[FILENAME_MAX] = "Results.txt";

    // declaration of an object CURL 
    CURL *handle;                   

    CURLcode result;              

    // set up the program environment that libcurl needs 
    curl_global_init(CURL_GLOBAL_ALL);
    
	// curl_easy_init() returns a CURL easy handle 
    handle = curl_easy_init();

    // if everything's all right with the easy handle... 
	if (handle)
	{
		while (true)
		{
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
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

				// perform, then store the expected code in result
				result = curl_easy_perform(handle);

				if (result != CURLE_OK)
				{
					// if errors have occured, tell us what is wrong with result
					fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
					return 1;
				}

				char cKey[] = "CrumbStore\":{\"crumb\":\"";
				char *ptr1 = strstr(data.memory, cKey);
				char *ptr2 = ptr1 + strlen(cKey);
				char *ptr3 = strstr(ptr2, "\"}");
				if ( ptr3 != NULL )
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
				data.size= 0;
			}
			
			if (itr == symbolList.end())
				break;

			string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
			string symbol = *itr;
			string urlB = "?period1=";
			string urlC = "&period2=";
			string urlD = "&interval=1d&events=history&crumb=";
			string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
			const char * cURL = url.c_str();
			const char * cookies = sCookies.c_str();
			curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
			curl_easy_setopt(handle, CURLOPT_URL, cURL);

            // 这部分我直接注释掉了  这部分的作用是用来写file 我们不需要
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
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);//我们只需要这儿的write data2
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
			result = curl_easy_perform(handle);
			if (result != CURLE_OK)
			{
				// if errors have occurred, tell us what is wrong with result
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			//这部分是老师注释的
			//stringstream sData;
			//sData.str(data.memory);
			//string line;
			//getline(sData, line);
			//cout << line << endl;
			//for (; getline(sData, line); )
			//	cout << line << endl;

			// 现在没有release date 我就先随便写个时间了
			stock* newstock = new stock(*itr, "2019-02-01");
			stringstream sData;
			sData.str(data.memory);
			string sValue, sDate;
			double dValue = 0;
			string line;
			getline(sData, line);

			
			while ( getline(sData, line) ) {
				//cout << line << endl;  先注释掉了 
				sDate = line.substr(0, line.find_first_of(','));
				line.erase(line.find_last_of(','));
				sValue = line.substr(line.find_last_of(',')+1);
				dValue = strtod(sValue.c_str(), NULL);
				newstock->adjustedprice.push_back(dValue);  //传进adjusted price
				newstock->alltime.push_back(sDate); //传进time
			 //   cout << sDate << " " << std::fixed << ::setprecision(6) << dValue << endl;   这也先注释掉了
			}
			stockmap.insert(STOCKPAIR(*itr, newstock)); //传进map里面
			itr++;
		}
		free(data.memory);
		data.size= 0;
	}
    else 
    {
            fprintf(stderr, "Curl init failed!\n");
            return 1;
    }

    // cleanup since you've used curl_easy_init  
	//这部分是用来cout一下这个map看看有没有问题的
	map <string, stock*>::iterator newitr;
	for (newitr = stockmap.begin(); newitr != stockmap.end(); newitr++) {
		cout << "stock name is " << newitr->first<<endl;
		cout << "its date is" << newitr->second->getdate()<<endl;
		for (int i = 0; i <= 30; i++)
			cout << newitr->second->alltime[i]<<"   "<< newitr->second->adjustedprice[i]<<endl;
	}


    curl_easy_cleanup(handle);

    // release resources acquired by curl_global_init() 
    curl_global_cleanup();
	
	end = clock();
	timep += (float)(end - start) / CLOCKS_PER_SEC;
	cout << "Time=" << timep;
    return 0;
}