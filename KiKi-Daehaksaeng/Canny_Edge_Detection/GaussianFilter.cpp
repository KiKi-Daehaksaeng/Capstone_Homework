#pragma once
#include "definition.h"

void make_kernel(Kernel_TYPE & kernel, int sigma) {
	double s = 0;
	int range = filter_size / 2;
	for (int i = 0; i < filter_size; i++) {
		for (int j = 0; j < filter_size; j++) {
			kernel[i][j] = exp(-((i - range) * (i - range) + (j - range) * (j - range)) / (2 * sigma * sigma)) / (2.0 * pi * sigma * sigma);
			s += kernel[i][j];
		}
	}

	for (int i = 0; i < filter_size; i++) {
		for (int j = 0; j < filter_size; j++) {
			kernel[i][j] /= s;
		}
	}
	
	return;
}

double convolution(Kernel_TYPE &kernel, BYTE image[][height], int i, int j) {
	int x, y;
	int range = kernel.size() / 2;
	double sum = 0;
	for (int a = -range; a < range+1; a++) {
		if (i + a < 0) x = 0;
		else {
			if (i + a > 255) x = 255;
			else x = i + a;
		}
		for (int b = -range; b < range+1; b++) {
			if (j + b < 0) y = 0;
			else {
				if (j + b > 255) y = 255;
				else y = j + b;
			}
			sum += kernel[a + range][b + range] * (int)image[x][y];
		}
	}

	return sum;
}

void filtering(Kernel_TYPE& kernel, BYTE image[][height], BYTE result[][height]) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result[i][j] = (BYTE)round(convolution(kernel, image, i, j));
		}
	}
}

void sobel_filter(BYTE gaussianed_image[][height], double gradient[][height], double theta[][height]) {
	Kernel_TYPE Kx = { {-1,0,1},{-2,0,2},{-1,0,1} };
	Kernel_TYPE Ky = { {1,2,1},{0,0,0},{-1,-2,-1} };

	double ix, iy;
	double max = 0, temp;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			ix = convolution(Kx, gaussianed_image, i, j);
			iy = convolution(Ky, gaussianed_image, i, j);

			gradient[i][j] = sqrt(ix * ix + iy * iy);
			if (max < gradient[i][j])max = gradient[i][j];
			temp = atan(iy / ix) * 180 / pi;
			if (temp < 0) theta[i][j] = temp + 180;
			else theta[i][j] = temp;
		}
	}
	
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			gradient[i][j] = round(gradient[i][j] / max * 255); 
		}
	}
}
