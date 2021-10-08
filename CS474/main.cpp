#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "image.h"

using namespace std;

void readImage(char fname[], ImageType &image);
void writeImage(char fname[], ImageType &image);

ImageType padding(char fname[], int size);
void salt(char fname[], int percent);

void smoothing(char fname[], int size, bool G);
void median(char fname[], int size, int percent);
void correlation(char image[], char pattern[]);
void unsharp(char fname[]);
void highboost(char fname[]);

int main(int argc, char *argv[]) {
	char boat[]    = "boat.pgm";
	char f_16[]    = "f_16.pgm";
	char lenna[]   = "lenna.pgm";
	char peppers[] = "peppers.pgm";
	char sf[]      = "sf.pgm";
	char image[]   = "image.pgm";
	char pattern[] = "pattern.pgm";

	// Part 1
	// correlation(image, pattern);

	// Part 2
	// smoothing(lenna, 7, 0);
	// smoothing(lenna, 15, 0);
	// smoothing(lenna, 7, 1);
	// smoothing(lenna, 15, 1);

	// Part 3
	// salt(lenna, 50);
	// char salted_l[] = "salted_l.pgm";
	// median(salted_l, 7, 50);
	// median(salted_l, 15, 50);
	// smoothing(salted_l, 7, 0);
	// smoothing(salted_l, 15, 0);

	// Part 4
	unsharp(lenna);
	highboost(lenna);

	return 0;
}

ImageType padding(char fname[], int size) {
	// size is the mask size, not the number of pads you need
	ImageType image(256, 256, 255);
	readImage(fname, image);
	ImageType newImage(256 + (size - 1), 256 + (size - 1), 255);
	int temp;
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			image.getPixelVal(i, j, temp);
			newImage.setPixelVal(i + (size / 2), j + (size / 2), temp);
		}
	}
	return newImage;
}

void unsharp(char fname[]) {
	// Initialize f
	ImageType baseImage(256, 256, 255);
	ImageType tempImage(256, 256, 255);
	readImage(fname, baseImage);

	// Initialize FLP
	smoothing(fname, 15, 1);
	char smoothedImage[] = "smooth_i_a_5.pgm";
	smoothedImage[7]     = fname[0];

	readImage(smoothedImage, tempImage);

	// Initialize and calculate Gmask
	ImageType maskImage(256, 256, 255);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			int temp1, temp2, temp3;
			baseImage.getPixelVal(i, j, temp1);
			tempImage.getPixelVal(i, j, temp2);
			temp3 = temp1 - temp2;
			maskImage.setPixelVal(i, j, temp3);
		}
	}

	// Initialize and calculate G
	ImageType finalImage(256, 256, 255);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			int temp1, temp2, temp3;
			baseImage.getPixelVal(i, j, temp1);
			maskImage.getPixelVal(i, j, temp2);
			temp3 = temp1 + (temp2 * 1);
			finalImage.setPixelVal(i, j, temp3);
		}
	}

	char unsharpImage[] = "unsharp_i_a_5.pgm";
	unsharpImage[8]     = fname[0];
	writeImage(unsharpImage, finalImage);
}

void highboost(char fname[]) {
	// Initialize f
	ImageType baseImage(256, 256, 255);
	ImageType tempImage(256, 256, 255);
	readImage(fname, baseImage);

	// Initialize FLP
	smoothing(fname, 15, 1);
	char smoothedImage[] = "smooth_i_a_5.pgm";
	smoothedImage[7]     = fname[0];

	readImage(smoothedImage, tempImage);

	// Initialize and calculate Gmask
	ImageType maskImage(256, 256, 255);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			int temp1, temp2, temp3;
			baseImage.getPixelVal(i, j, temp1);
			tempImage.getPixelVal(i, j, temp2);
			temp3 = temp1 - temp2;
			maskImage.setPixelVal(i, j, temp3);
		}
	}

	// Initialize and calculate G
	ImageType finalImage(256, 256, 255);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			int temp1, temp2, temp3;
			baseImage.getPixelVal(i, j, temp1);
			maskImage.getPixelVal(i, j, temp2);
			temp3 = temp1 + (temp2 * 2);
			finalImage.setPixelVal(i, j, temp3);
		}
	}

	char highboostImage[] = "highboost_i_a_5.pgm";
	highboostImage[10]    = fname[0];
	writeImage(highboostImage, finalImage);
}

void correlation(char image[], char pattern[]) {
	int rowsImage, colsImage;
	int rowsPattern, colsPattern;
	int levelsImage, levelsPattern;

	ImageType fullImage(442, 288, 255);
	ImageType patternImage(83, 55, 255);

	readImage(image, fullImage);
	readImage(pattern, patternImage);

	fullImage.getImageInfo(rowsImage, colsImage, levelsImage);
	patternImage.getImageInfo(rowsPattern, colsPattern, levelsPattern);

	vector<int> corr;

	int temp;

	// iterate through image pixels
	for (int i = 0; i < rowsImage; i++) {
		for (int j = 0; j < colsImage; j++) {
			int sum   = 0;
			int temp2 = 0;
			int temp3 = 0;

			// iterate over mask
			for (int k = -rowsPattern / 2; k < rowsPattern / 2; k++) {
				for (int l = -colsPattern / 2; l < colsPattern / 2; l++) {
					if (k < 0 || i + k >= rowsPattern || l < 0 || j + l >= colsPattern)
						sum += 0;
					else {
						patternImage.getPixelVal(k, l, temp2);
						fullImage.getPixelVal(k + i, j + l, temp3);
						sum += temp2 * temp3;
					}
				}
			}
			corr.push_back(sum);
		}
	}

	int temp4;

	int max = 0;
	int min = 1000000000;
	int value;

	// find min an max correlation values
	for (int i = 0; i < rowsImage; i++) {
		for (int j = 0; j < colsImage; j++) {
			value = corr[i * 288 + j];

			if (value > max) { max = value; }
			if (value < min) min = value;
		}
	}

	ImageType fullImageF(442, 288, 255);
	char newfname[] = "correlatedImage.pgm";
	for (int i = 0; i < rowsImage; i++) {
		for (int j = 0; j < colsImage; j++) { fullImageF.setPixelVal(i, j, 255 * corr[i * 288 + j] / (max - min)); }
	}

	writeImage(newfname, fullImageF);
}

void salt(char fname[], int percent) {
	char newfname[] = "salted_i.pgm";
	newfname[7]     = fname[0];

	ImageType image(256, 256, 255);
	readImage(fname, image);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if (rand() % 100 + 1 <= percent) {
				if (rand() % 2 + 1 == 1) {
					image.setPixelVal(i, j, 0);
				} else {
					image.setPixelVal(i, j, 255);
				}
			}
		}
	}
	writeImage(newfname, image);
}

static int g7[7][7] = {{1, 1, 2, 2, 2, 1, 1}, {1, 2, 2, 4, 2, 2, 1}, {2, 2, 4, 8, 4, 2, 2}, {2, 4, 8, 16, 8, 4, 2},
                       {2, 2, 4, 8, 4, 2, 2}, {1, 2, 2, 4, 2, 2, 1}, {1, 1, 2, 2, 2, 1, 1}};

static int g15[15][15] = {
    {2, 2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2},
    {2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2},
    {3, 4, 6, 7, 9, 10, 10, 11, 10, 10, 9, 7, 6, 4, 3},
    {4, 5, 7, 9, 10, 12, 13, 13, 13, 12, 10, 9, 7, 5, 4},
    {5, 7, 9, 11, 13, 14, 15, 16, 15, 14, 13, 11, 9, 7, 5},
    {5, 7, 10, 12, 14, 16, 17, 18, 17, 16, 14, 12, 10, 7, 5},
    {6, 8, 10, 13, 15, 17, 19, 19, 19, 17, 15, 13, 10, 8, 6},
    {6, 8, 11, 13, 16, 18, 19, 20, 19, 18, 16, 13, 11, 8, 6},
    {6, 8, 10, 13, 15, 17, 19, 19, 19, 17, 15, 13, 10, 8, 6},
    {5, 7, 10, 12, 14, 16, 17, 18, 17, 16, 14, 12, 10, 7, 5},
    {5, 7, 9, 11, 13, 14, 15, 16, 15, 14, 13, 11, 9, 7, 5},
    {4, 5, 7, 9, 10, 12, 13, 13, 13, 12, 10, 9, 7, 5, 4},
    {3, 4, 6, 7, 9, 10, 10, 11, 10, 10, 9, 7, 6, 4, 3},
    {2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2},
    {2, 2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2},
};

void smoothing(char fname[], int size, bool G) {
	ImageType originalImage = padding(fname, size);
	ImageType smoothImage(256, 256, 255);

	char newfname[] = "smooth_i_i_i.pgm";
	newfname[7]     = fname[0];
	if (G) {
		newfname[9] = 'G';
	} else {
		newfname[9] = 'A';
	}
	newfname[11] = '0' + (size % 10);

	int sum    = 0;
	int factor = 0;
	int temp;
	if (!G) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 256; j++) {  // for each pixel in the original image
				for (int k = i; k < i + size; k++) {
					for (int l = j; l < j + size; l++) {  // for each weight in the mask
						originalImage.getPixelVal(k, l, temp);
						sum = sum + temp;
						// cout << k << ", " << l << endl;
					}
				}
				// calculate sum
				sum = sum / (size * size);
				smoothImage.setPixelVal(i, j, sum);
				// cout << "Pixel " << i << ", " << j << endl;
				sum = 0;
			}
		}
	} else if (size == 7) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 256; j++) {  // for each pixel in the original image
				for (int k = i; k < i + size; k++) {
					for (int l = j; l < j + size; l++) {  // for each weight in the mask
						originalImage.getPixelVal(k, l, temp);
						sum    = sum + (temp * g7[k - i][l - j]);
						factor = factor + g7[k - i][l - j];
						// cout << k << ", " << l << endl;
					}
				}
				// calculate sum
				sum = sum / factor;
				smoothImage.setPixelVal(i, j, sum);
				// cout << "Pixel " << i << ", " << j << endl;
				sum    = 0;
				factor = 0;
			}
		}
	} else if (size == 15) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 256; j++) {  // for each pixel in the original image
				for (int k = i; k < i + size; k++) {
					for (int l = j; l < j + size; l++) {  // for each weight in the mask
						originalImage.getPixelVal(k, l, temp);
						sum    = sum + (temp * g15[k - i][l - j]);
						factor = factor + g15[k - i][l - j];
						// cout << k << ", " << l << endl;
					}
				}
				// calculate sum
				sum = sum / factor;
				smoothImage.setPixelVal(i, j, sum);
				// cout << "Pixel " << i << ", " << j << endl;
				sum    = 0;
				factor = 0;
			}
		}
	}
	writeImage(newfname, smoothImage);
}

void median(char fname[], int size, int percent) {
	ImageType originalImage = padding(fname, size);
	ImageType medianImage(256, 256, 255);

	char newfname[] = "median_i_i_ii.pgm";
	newfname[7]     = fname[7];
	newfname[9]     = '0' + (size % 10);
	newfname[11]    = '0' + (percent / 10);
	newfname[12]    = '0' + (percent % 10);

	int array[size * size];
	int n = sizeof(array) / sizeof(array[0]);

	int factor = 0;
	int temp;
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {  // for each pixel in the original image
			for (int k = i; k < i + size; k++) {
				for (int l = j; l < j + size; l++) {  // for each weight in the mask
					originalImage.getPixelVal(k, l, temp);
					array[((k - i) * size) + ((l - j) % size)] = temp;
				}
			}
			sort(array, array + n);
			temp = array[(size * size) / 2];
			medianImage.setPixelVal(i, j, temp);
			cout << "Pixel " << i << ", " << j << endl;
			temp = 0;
		}
	}
	writeImage(newfname, medianImage);
}