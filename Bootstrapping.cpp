
#include "Bootstrapping.h"

double Bootstrapping::Std(Vector& series) const {
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
