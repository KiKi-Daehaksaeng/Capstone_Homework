#pragma once
#include "definition.h"

void nonMaximunSupperssion(double image[][height], double theta[][height], double result[][height]) {
	double q, r;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
				result[i][j] = image[i][j];
				continue;
			}

			if ((theta[i][j] >= 0 && theta[i][j] < 22.5) || (theta[i][j] >= 157.5 && theta[i][j] <= 180)) {
				q = image[i][j + 1];
				r = image[i][j - 1];
			}
			else if (theta[i][j] >= 22.5 && theta[i][j] <67.5) {
				q = image[i+1][j - 1];
				r = image[i-1][j + 1];
			}
			else if (theta[i][j] >= 67.5 && theta[i][j] < 112.5) {
				q = image[i+1][j];
				r = image[i-1][j];
			}
			else if (theta[i][j] >= 112.5 && theta[i][j] < 157.5) {
				q = image[i-1][j - 1];
				r = image[i+1][j + 1];
			}

			if (image[i][j] >= q && image[i][j] >= r) result[i][j] = image[i][j];
			else  result[i][j] = 0;
		}
	}
}