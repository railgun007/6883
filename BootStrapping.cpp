#include "BootStrapping.h"


void BootStrapping::set_benchmark(StockData& data_container, string bench_mark_name)
{
	benchmark.clear();
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
	FILE* fp = NULL;

	// name of files  
	const char outfilename[FILENAME_MAX] = "Output.txt";
	const char resultfilename[FILENAME_MAX] = "Results.txt";

	// declaration of an object CURL
	CURL* handle;

	CURLcode result;

	// set up the program environment that libcurl needs 
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();
	if (handle)
	{
		stock* new_bench_mark = new stock(bench_mark_name, "");
		string startTime = to_string(smallest_time_sec);
		string endTime = to_string(largest_time_sec);
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
				return;
			}

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

			// perform, then store the expected code in result
			result = curl_easy_perform(handle);

			if (result != CURLE_OK)
			{
				// if errors have occured, tell us what is wrong with result
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return;
			}

			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char* ptr1 = strstr(data.memory, cKey);
			if (ptr1 == NULL)
				throw - 1;
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
		Download_data(new_bench_mark, bench_mark_name, startTime, endTime, data, outfilename, handle, fp, result, sCookies, sCrumb);
		benchmark.insert(pair<string, pair<double, double>>(new_bench_mark->alltime[0], pair<double, double>(new_bench_mark->adjustedprice[0], NAN)));
		for (int i = 1; i < new_bench_mark->adjustedprice.size(); i++)
		{
			benchmark.insert(pair<string, pair<double, double>>(new_bench_mark->alltime[i], pair<double, double>(new_bench_mark->adjustedprice[i],
				(new_bench_mark->adjustedprice[i] - new_bench_mark->adjustedprice[i - 1]) / new_bench_mark->adjustedprice[i - 1])));
		}
		delete new_bench_mark;
		new_bench_mark = NULL;
	}
}

void BootStrapping::print_benchmark() const
{
	cout << "benchmark : " << endl;
	for_each(benchmark.begin(), benchmark.end(), [](pair<string, pair<double, double>> ele)
		{cout << ele.first << "\t" << ele.second.first << "\t" << ele.second.second << endl; });
}

double BootStrapping::Stdev(Vector& series) const
{
	double sum = 0;
	for (int i = 0; i < series.size(); i++) {
		sum = sum + series[i];
	}
	double avg = sum / series.size();
	double sqrsum = 0;
	for (int j = 0; j < series.size(); j++) {
		sqrsum = sqrsum + (series[j] - avg) * (series[j] - avg);
	}
	return sqrt(sqrsum / series.size());
}
double BootStrapping::Mean(Vector& series) const
{
	double sum = 0;
	for_each(series.begin(), series.end(), [&](auto ele) {sum += ele; });
	return sum / series.size();
}

void BootStrapping::Divide_vector(const vector<pair<string, string>>& vec)
{
	groups.clear();
	groups.resize(num_group);
	for (int i = 0; i < num_group - 1; i++)//fill 0~n-2 rows in groups with fixed length
	{
		for (int j = (int)((double)i * (double)vec.size() / (double)num_group); j < (int)(((double)i + 1) * (double)vec.size() / (double)num_group); j++)
		{
			groups[i].push_back(vec[j]);
		}
	}
	for (int j = (int)(((double)num_group - 1) * (double)vec.size() / (double)num_group); j < vec.size(); j++)// fill the rest of vec into n-1(last) row
	{
		groups[num_group - 1].push_back(vec[j]);
	}
}

void BootStrapping::Calculate(HMatrix& return_mat, StockData& data_container, int sampling_num, int sampling_times)
{
	const string date_suffix = "T16:00:00";
	const int date_num = 30;
	return_mat.resize(num_group);
	vector<string> group_name; //the name of three groups
	group_name.push_back("Miss estimate group");
	group_name.push_back("Meat estimate group");
	group_name.push_back("Beat estimate group");
	for (int group_cont = 0; group_cont < num_group; group_cont++)//for each group (i.e. best/media/worst group)
	{
		Matrix AARt_mat(2 * date_num, Vector(sampling_times, 0));
		Matrix CAAR_mat(2 * date_num, Vector(sampling_times, 0));
		for (int j = 0; j < sampling_times; j++)//for each time's sampling
		{
			vector<const pair<string, string>*> sample_name;
			Sampling_name<pair<string, string>>(sample_name, groups[group_cont], sampling_num);//sample_name contains name,release_date of stocks
			Matrix AAit(2 * date_num, Vector(sampling_num, 0));//(60*30) matrix  60days AR value of 30 stocks
			int AAit_j = 0;
			for (auto itr = sample_name.begin(); itr != sample_name.end(); itr++)//for each stock in the bootstrapping sampling list
			{
				stock* current_stock_ptr = data_container.stock_map[(*itr)->first];
				current_stock_ptr->group_number = group_name[group_cont]; //Get the group name of 
				string search_date = (*itr)->second;//search the releash data within the vector
				int start_idx, end_idx;
				auto search_itr = find(current_stock_ptr->alltime.begin(), current_stock_ptr->alltime.end(), search_date);
				if (search_itr != current_stock_ptr->alltime.end()) //means the data of release date can be found in yahoo
				{// Since the length of data set is larger than 60, we need to find the start index and end index
					start_idx = distance(current_stock_ptr->alltime.begin(), search_itr) - date_num;
					current_stock_ptr->start_index = start_idx;
					end_idx = distance(current_stock_ptr->alltime.begin(), search_itr) + date_num;
					current_stock_ptr->end_index = end_idx;
				}
				else//if the data of release date cannot be found in yahoo, we use the day before the release until we find it
				{
					vector<int> alltime_sec;
					for_each(current_stock_ptr->alltime.begin(), current_stock_ptr->alltime.end(), [&](auto ele)
						{
							alltime_sec.push_back(stoi(getTimeinSeconds(ele + date_suffix)));
						}
					);
					int search_date_sec = stoi(getTimeinSeconds(search_date + date_suffix));
					auto search_sec_itr = find(alltime_sec.begin(), alltime_sec.end(), search_date_sec);
					while (search_sec_itr == alltime_sec.end())//while loop backward until search out a date existing in stock data
					{
						search_date_sec -= 86400;
						search_sec_itr = find(alltime_sec.begin(), alltime_sec.end(), search_date_sec);
					}
					start_idx = distance(alltime_sec.begin(), search_sec_itr) - date_num;
					current_stock_ptr->start_index = start_idx;
					end_idx = distance(current_stock_ptr->alltime.begin(), search_itr) + date_num;
					current_stock_ptr->end_index = end_idx;
				}
				int AAit_i = 0;
				for (int i = start_idx + 1; i < end_idx + 1; i++)//for each day, calculate the abnormal return of one stock
				{
					if (isnan(current_stock_ptr->abnormal_return[i]))//if nan, then calculate and store it
					{
						double stock_r = (current_stock_ptr->adjustedprice[i] - current_stock_ptr->adjustedprice[i - 1]) / current_stock_ptr->adjustedprice[i - 1];
						current_stock_ptr->abnormal_return[i] = stock_r - benchmark[(*itr)->second].second;
					}
					AAit[AAit_i][AAit_j] = current_stock_ptr->abnormal_return[i];
					AAit_i++;
				}
				AAit_j++;
			}
			AARt_mat[0][j] = Mean(AAit[0]);//take average of abnormal return of 30 stocks
			CAAR_mat[0][j] = Mean(AAit[0]);//take average of cumulative abnormal return 30 stocks
			for (int k = 1; k < 60; k++)
			{
				AARt_mat[k][j] = Mean(AAit[k]); //put the average of 30 stocks into the big matrix
				CAAR_mat[k][j] = CAAR_mat[k - 1][j] + AARt_mat[k][j];
			}
		}
		Vector AAR(AARt_mat.size(), 0);  //a vector of 60 days, each value in it is the average of AAR of 30 bootstrapping samplings
		Vector AAR_SD(AARt_mat.size(), 0);//a vector of 60 days, each value in it is the standard deviation of AAR of 30 bootstrapping samplings
		Vector CAAR(CAAR_mat.size(), 0);//a vector of 60 days, each value in it is the average of CAAR of 30 bootstrapping samplings
		Vector CAAR_SD(CAAR_mat.size(), 0);//a vector of 60 days, each value in it is the standard deviation of CAAR of 30 bootstrapping samplings
		for (int k = 0; k < 2 * date_num; k++)
		{
			AAR[k] = Mean(AARt_mat[k]);
			AAR_SD[k] = Stdev(AARt_mat[k]);
			CAAR[k] = Mean(CAAR_mat[k]);
			CAAR_SD[k] = Stdev(CAAR_mat[k]);
		}
		Matrix temp = { AAR,AAR_SD,CAAR,CAAR_SD }; //these are four indexs of one group
		return_mat[group_cont] = temp;
	}
}