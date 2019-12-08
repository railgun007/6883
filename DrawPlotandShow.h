#ifndef DrawPlotandShow_h
#define DrawPlotandShow_h
#include <stdio.h>
#include "Matrix.h"

void Plot_results(double* xData, double* yData, double* yData2, double* yData3, int dataSize);
void Choose_to_show(int which_group, int which_type, const HMatrix &res_mat);
void Plot_CAAR(const HMatrix& res_mat);
void name_group(int);
void name_type(int);
#endif  