#ifndef DrawPlotandShow_h
#define DrawPlotandShow_h
#include <stdio.h>
#include "Matrix.h"

void Choose_to_show(int which_group, int which_type, const HMatrix& res_mat);  //choose one of four indexs and one of three group to show
void Plot_CAAR(const HMatrix& res_mat, const string& gnuplot_address);//plot the CAAR for three groups
void name_group(int);//it helps to choose and show name of three groups
void name_type(int);//it helps to choose and show name of four indexs
#endif 