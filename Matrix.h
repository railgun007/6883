#ifndef Matrix_h
#define Matrix_h

#include <vector>
#include <iostream>
using namespace std;
typedef vector<double> Vector;
typedef vector<Vector> Matrix;
typedef vector<Matrix> HMatrix;

Vector operator*(const Matrix& C,const Vector& V);
Vector operator*(const double& a,const Vector& V);
Vector operator+(const double& a,const Vector& V);
Vector operator+(const Vector& V,const Vector& W);
Vector operator/(const Vector& V,const Vector& W);
Vector exp(const Vector& V);
double operator^(const Vector& V,const Vector& W);
ostream & operator<<(ostream & out, Vector & V);
ostream & operator<<(ostream & out, Matrix & W);
ostream& operator<<(ostream& out, HMatrix& HM);

#endif
