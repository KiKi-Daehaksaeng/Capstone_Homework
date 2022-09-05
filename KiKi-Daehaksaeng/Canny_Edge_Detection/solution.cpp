#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "definition.h"

#define src1 "Suzie_CIF_352x288.raw"
#define src2 "barbara(512x512).raw"
#define src3 "gLenna256_256.raw"

#define output1 "output_Suzie_CIF_352x288.raw"
#define output2 "output_barbara(512x512).raw"
#define output3 "output_gLenna256_256.raw"

extern int sigma = 1;
extern double high = 0.09, low = 0.05;


BYTE image[width][height], gaussian[width][height], threshold[width][height], result[width][height];
double gradient[width][height], theta[width][height], nonMaxSuppress[width][height];

void solve() {
	ifstream src;

	src.open(src3, ifstream::binary);
	if (!src) {
		cout << "file not exists";
		return;
	}
	Kernel_TYPE kernel(5, vector<double>(5));

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++)
			src.get((char&)image[i][j]);
	}
	src.close();

	make_kernel(kernel, sigma);

	filtering(kernel, image, gaussian);

	sobel_filter(gaussian, gradient, theta);

	nonMaximunSupperssion(gradient, theta, nonMaxSuppress);

	double_threshold(nonMaxSuppress, low, high, threshold);

	hysteresis(threshold, result);

	ofstream output;
	output.open(output3, ofstream::binary);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++)
			output.put(BYTE(result[i][j]));
	}
	output.close();
}