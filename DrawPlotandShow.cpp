#include "DrawPlotandShow.h"


void Plot_results(double* xData, double* yData, double* yData2, double* yData3, int dataSize) {
	FILE* gnuplotPipe, * tempDataFile;
	const char* tempDataFileName1 = "Best";
	const char* tempDataFileName2 = "media";
	const char* tempDataFileName3 = "Worst";
	double x, y, x2, y2, x3, y3;
	int i;
	gnuplotPipe = _popen("D:/Software/gnuplot/bin/gnuplot.exe", "w");
	if (gnuplotPipe) {
		fprintf(gnuplotPipe, "plot \"%s\" with lines,  \"%s\" with lines,  \"%s\" with lines\n ", tempDataFileName1, tempDataFileName2, tempDataFileName3);
		fflush(gnuplotPipe);

		tempDataFile = fopen(tempDataFileName1, "w");
		for (i = 0; i <= dataSize; i++) {
			x = xData[i];
			y = yData[i];
			fprintf(tempDataFile, "%lf %lf\n", x, y);
		}
		fclose(tempDataFile);

		tempDataFile = fopen(tempDataFileName2, "w");
		for (i = 0; i <= dataSize; i++) {
			x2 = xData[i];
			y2 = yData2[i];
			fprintf(tempDataFile, "%lf %lf\n", x2, y2);
		}
		fclose(tempDataFile);

		tempDataFile = fopen(tempDataFileName3, "w");
		for (i = 0; i <= dataSize; i++) {
			x3 = xData[i];
			y3 = yData3[i];
			fprintf(tempDataFile, "%lf %lf\n", x3, y3);
		}
		fclose(tempDataFile);

		printf("press enter to continue...");
		getchar();
		remove(tempDataFileName1);
		remove(tempDataFileName2);
		remove(tempDataFileName3);
		fprintf(gnuplotPipe, "exit \n");
	}
	else {
		printf("gnuplot not found...");
	}
}






void Choose_to_show(int which_group, int which_type, const HMatrix& res_mat) {
	name_type(which_type);
	name_group(which_group);
	cout << endl;
	for (int i = 0; i <= 59; i++) cout << res_mat[which_group - 1][which_type - 1][i] << endl;
}void Plot_CAAR(const HMatrix& res_mat) {
	int i = 0;
	int nIntervals = 59;
	double intervalSize = 1.0;
	double stepSize = intervalSize / nIntervals;
	double* xData = (double*)malloc((nIntervals + 1) * sizeof(double));
	double* yData = (double*)malloc((nIntervals + 1) * sizeof(double));
	double* yData2 = (double*)malloc((nIntervals + 1) * sizeof(double));
	double* yData3 = (double*)malloc((nIntervals + 1) * sizeof(double));
	xData[0] = 0.0;
	double x0 = 0.0;
	for (i = 0; i < nIntervals; i++) {
		x0 = xData[i];
		xData[i + 1] = x0 + stepSize;
	}


	for (i = 0; i <= nIntervals; i++) {

		yData[i] = res_mat[0][2][i];
	}
	for (i = 0; i <= nIntervals; i++) {

		yData2[i] = res_mat[1][2][i];
	}
	for (i = 0; i <= nIntervals; i++) {

		yData3[i] = res_mat[2][2][i];
	}
	Plot_results(xData, yData, yData2, yData3, nIntervals);
}


void name_type(int which_type) {
	enum type {
		AAR,
		AAR_SD,
		CAAR,
		CAAR_SD
	};
	switch (which_type) {
	case AAR:
		cout << "The AAR_SD for";
		break;
	case AAR_SD:
		cout << "The AAR_SD for ";
		break;
	case CAAR:
		cout << "The CAAR for ";
		break;
	case CAAR_SD:
		cout << "The CAAR_SD for";
		break;
	}
}


void name_group(int which_group) {
	enum type {
		Miss_Estimate_Group,
		Meet_Estimate_Group,
		Beat_Estimate_Group
	};
	switch (which_group) {
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