#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

#define pi 3.1415926535
#define filter_size 5
#define BYTE unsigned char
#define Kernel_TYPE vector<vector<double>>

//width and height of source Image(if it has differnt size, change them)
#define width 256
#define height 256

//GaussianFIlter.cpp
void make_kernel(Kernel_TYPE& kernel, int sigma);
double convolution(Kernel_TYPE& kernel, BYTE image[][height], int i, int j);
void filtering(Kernel_TYPE& kernel, BYTE image[][height], BYTE result[][height]);
void sobel_filter(BYTE gaussianed_image[][height], double gradient[][height], double theta[][height]);

//NonMaximumSuppression.cpp
void double_threshold(double image[][height], double low, double high, BYTE result[][height]);
void hysteresis(BYTE image[][height], BYTE result[][height]);

//DoubleThreshold.cpp
void nonMaximunSupperssion(double image[][height], double theta[][height], double result[][height]);

//solution.cpp
void solve();