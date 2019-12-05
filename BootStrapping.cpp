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

void BootStrapping::Divide_vector(const vector<pair<string, string>>& vec)
{
	groups.clear();
	groups.resize(num_group);
	for (int i = 0; i < num_group - 1; i++)//fill 0~n-2 rows in groups with fixed length
	{
		for (int j = (int)(i * vec.size() / (double)num_group); j < (int)((i + 1) * vec.size() / (double)num_group); j++)
		{
			groups[i].push_back(vec[j].first);
		}
	}
	for (int j = (int)((num_group - 1) * vec.size() / (double)num_group); j < vec.size(); j++)// fill the rest of vec into n-1(last) row
	{
		groups[num_group - 1].push_back(vec[j].first);
	}
}
void BootStrapping::Sampling_name(vector<string*>& return_vec, vector<string>& group, int sampling_num) const
{
	return_vec.resize(sampling_num);
	if (group.size() < sampling_num) { return; }
	vector<int> vect;
	for (int i = 0; i < group.size(); i++) vect.push_back(i);
	random_shuffle(vect.begin(), vect.end());
	for (int i = 0; i < return_vec.size(); i++) 
	{
		return_vec[i] = &group[vect[i]];
	}
}