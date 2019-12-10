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
		Download_data(new_bench_mark, bench_mark_name, startTime, endTime, data, outfilename, handle, fp, result);
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
	for (int group_cont = 0; group_cont < num_group; group_cont++)//for each group (i.e. best/media/worst group)
	{
		Matrix AARt_mat(2 * date_num, Vector(sampling_times, 0));
		Matrix CAAR_mat(2 * date_num, Vector(sampling_times, 0));
		for (int j = 0; j < sampling_times; j++)//for each time's sampling
		{
			vector<const pair<string, string>*> sample_name;
			Sampling_name<pair<string, string>>(sample_name, groups[group_cont], sampling_num);//sample_name contains name,release_date of stocks
			Matrix AAit(2 * date_num, Vector(sampling_num, 0));//(60*30) matrix
			int AAit_j = 0;
			for (auto itr = sample_name.begin(); itr != sample_name.end(); itr++)//for each stock
			{
				stock* current_stock_ptr = data_container.stock_map[(*itr)->first];
				current_stock_ptr->group_number = group_cont;
				string search_date = (*itr)->second;
				int start_idx;
				auto search_itr = find(current_stock_ptr->alltime.begin(), current_stock_ptr->alltime.end(), search_date);
				if (search_itr != current_stock_ptr->alltime.end())
				{
					start_idx = distance(current_stock_ptr->alltime.begin(), search_itr) - date_num;
					current_stock_ptr->start_index = start_idx;
					int end_idx = distance(current_stock_ptr->alltime.begin(), search_itr) + date_num;
					current_stock_ptr->end_index = end_idx;
				}
				else
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
					int end_idx = distance(current_stock_ptr->alltime.begin(), search_itr) + date_num;
					current_stock_ptr->end_index = end_idx;
				}

				int AAit_i = 0;
				for (int i = start_idx + 1; i < start_idx + 2 * date_num + 1; i++)//for each day
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
			Vector sample_AARt(AAit.size(), 0);
			Vector sample_CAAR(AAit.size(), 0);
			sample_AARt[0] = Mean(AAit[0]);
			sample_CAAR[0] = sample_AARt[0];
			for (int k = 1; k < AAit.size(); k++)
			{
				sample_AARt[k] = Mean(AAit[k]);
				sample_CAAR[k] = sample_CAAR[k - 1] + sample_AARt[k];
			}
			/*AARt = (1 / (j + 1)) * (j * AARt + sample_AARt);
			CAAR = (1 / (j + 1)) * (j * CAAR + sample_CAAR);*/
			for (int k = 0; k < 2 * date_num; k++)
			{
				AARt_mat[k][j] = sample_AARt[k];
				CAAR_mat[k][j] = sample_CAAR[k];
			}
		}
		Vector AAR(AARt_mat.size(), 0);
		Vector AAR_SD(AARt_mat.size(), 0);
		Vector CAAR(CAAR_mat.size(), 0);
		Vector CAAR_SD(CAAR_mat.size(), 0);
		for (int k = 0; k < 2 * date_num; k++)
		{
			AAR[k] = Mean(AARt_mat[k]);
			AAR_SD[k] = Stdev(AARt_mat[k]);
			CAAR[k] = Mean(CAAR_mat[k]);
			CAAR_SD[k] = Stdev(CAAR_mat[k]);
		}
		Matrix temp = { AAR,AAR_SD,CAAR,CAAR_SD };
		return_mat[group_cont] = temp;
	}
}