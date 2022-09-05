#pragma once
#include "definition.h"
extern int strong = 255, weak = 25;

void double_threshold(double image[][height],double low, double high, BYTE result[][height]) {
	double max = 0, highThreshold, lowThreshold;
	for (int i = 0; i < width; i++) {
		double temp = *max_element(image[i], image[i] + height);
		if (max < temp) max = temp;
	}

	highThreshold = max * high;
	lowThreshold = highThreshold * low;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (image[i][j] >= highThreshold) result[i][j] = 255;
			else if (image[i][j] >= lowThreshold)result[i][j] = 25;
		}
	}
}

void hysteresis(BYTE image[][height],BYTE result[][height]) {
	for (int i = 1; i < width-1; i++) {
		for (int j = 1; j < height-1; j++) {
			if (image[i][j]>25) {
				if (image[i + 1][j - 1] == 255 || image[i + 1][j] == 255 || image[i + 1][j + 1] == 255 ||
					image[i][j - 1] == 255 || image[i][j + 1] == 255 || image[i - 1][j - 1] == 255 ||
					image[i - 1][j] == 255 || image[i - 1][j + 1] == 255)
					result[i][j] = 255;
				else result[i][j] = 0;
			}
		}
	}
}