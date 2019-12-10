#include "DrawPlotandShow.h"





void Choose_to_show(int which_group, int which_type, const HMatrix& res_mat) {
	name_type(which_type);
	name_group(which_group);
	cout << endl;
	for (int i = 0; i <= 59; i++) cout << res_mat[which_group - 1][which_type - 1][i] << endl;
}

void Plot_CAAR(const HMatrix& res_mat) {

	getchar();
	int nIntervals = 59;
	int stepSize = 1;

	int* DateRange = (int*)malloc((nIntervals + 1) * sizeof(int));

	DateRange[0] = -29;
	int x0;
	for (int i = 0; i < nIntervals; i++)
	{
		x0 = DateRange[i];
		DateRange[i + 1] = x0 + stepSize;
	}

	FILE* gnuplotPipe, * tempDataFile;
	const char* tempDataFileName1 = "MISS";
	const char* tempDataFileName2 = "Meet";
	const char* tempDataFileName3 = "Beat";
	double x1, y1, x2, y2, x3, y3;

	gnuplotPipe = _popen("D:/Software/gnuplot/bin/gnuplot.exe", "w");
	if (gnuplotPipe)
	{
		fprintf(gnuplotPipe, "plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempDataFileName1,
			tempDataFileName2,
			tempDataFileName3);
		fflush(gnuplotPipe);

		tempDataFile = fopen(tempDataFileName1, "w");
		for (int i = 0; i <= nIntervals; i++)
		{
			x1 = DateRange[i];
			y1 = res_mat[0][2][i];
			fprintf(tempDataFile, "%lf %lf\n", x1, y1);
		}
		fclose(tempDataFile);

		tempDataFile = fopen(tempDataFileName2, "w");
		for (int i = 0; i <= nIntervals; i++)
		{
			x2 = DateRange[i];
			y2 = res_mat[1][2][i];
			fprintf(tempDataFile, "%lf %lf\n", x2, y2);
		}
		fclose(tempDataFile);

		tempDataFile = fopen(tempDataFileName3, "w");
		for (int i = 0; i <= nIntervals; i++)
		{
			x3 = DateRange[i];
			y3 = res_mat[2][2][i];
			fprintf(tempDataFile, "%lf %lf\n", x3, y3);
		}
		fclose(tempDataFile);

		cout << endl;
		printf("# Press enter to continue ... # ");
		getchar();
		remove(tempDataFileName1);
		remove(tempDataFileName2);
		remove(tempDataFileName3);
		fprintf(gnuplotPipe, "exit \n");

	}
	else
	{
		printf("gnuplot not found...");
	}
}


void name_type(int which_type) {
	enum type {
		AAR,
		AAR_SD,
		CAAR,
		CAAR_SD
	};
	switch (which_type - 1) {
	case AAR:
		cout << "The AAR for ";
		break;
	case AAR_SD:
		cout << "The AAR_SD for ";
		break;
	case CAAR:
		cout << "The CAAR for ";
		break;
	case CAAR_SD:
		cout << "The CAAR_SD for ";
		break;
	}
}


void name_group(int which_group) {
	enum type {
		Miss_Estimate_Group,
		Meet_Estimate_Group,
		Beat_Estimate_Group
	};
	switch (which_group - 1) {
	case Miss_Estimate_Group:
		cout << "Miss_Estimate_Group is";
		break;
	case Meet_Estimate_Group:
		cout << "Meet_Estimate_Group is ";
		break;
	case Beat_Estimate_Group:
		cout << "Beat_Estimate_Group is ";
	}
}