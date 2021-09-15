#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "image.h"

using namespace std;

void readImage(char fname[], ImageType& image);
void writeImage(char fname[], ImageType& image);
void sample (char fname[]);
void eqHistogram(char fname[]);

int main(int argc, char * argv[])
{
    /*int val;

    ImageType image(256, 256, 255);
    readImage(argv[1], image);
    image.getPixelVal(2, 2, val);
    writeImage(argv[2], image);*/

    //part 1
    // char lenna[] = "lenna.pgm";
    // char peppers[] = "peppers.pgm";
    // sample(lenna);
    // sample(peppers);

    //part 3
    char boat[] = "boat.pgm";
    char f_16[] = "f_16.pgm";
    eqHistogram(boat);

    return 0;
}

void eqHistorgram(char fname[])
{
    ImageType image(256, 256, 255);
    readImage(fname, image);

    int n_bins = 255 + 1;
    int total = 256 * 256;

    // Compute histogram
    vector<int> hist(n_bins, 0);
    for (int i = 0; i < 256; i++) {
        for(int j = 0; j < 256; j++){
            int temp;
            hist[image.getPixelVal(i, j, temp),0]++;
        }
    }

    //Find first nonzero bin
    int i = 0;
    while(!hist[i]) ++i;

    if (hist[i] == total) {
        for (int j = 0; j < 256; j++) { 
            for(int k = 0; k < 256; k++){
                int temp;
                image.setPixelVal(j, k, i); 
            }
        }
        return;
    }

    // Compute scale
    float scale = (n_bins - 1.f) / (total - hist[i]);

    // Initialize lut
    vector<int> lut(n_bins, 0);
    i++;

    int sum = 0;
    for (; i < hist.size(); ++i) {
        sum += hist[i];
        // the value is saturated in range [0, max_val]
        lut[i] = max(0, min(int(round(sum * scale)), 255));
    }

    ImageType equalizedImage(256, 256, 255);

    // Apply equalization
    for (int i = 0; i < 256; i++) {
        for(int j = 0; j < 256; j++){
            int temp;
            equalizedImage.setPixelVal(i, j, lut[image.getPixelVal(i,j,temp), 0]);
        }
    }
    char name[] = "in.pgm";
    name[0] = '0' + i;
    name[1] = fname[0];

    writeImage(name, equalizedImage);

}

void sample(char fname[]){
    ImageType image(256, 256, 255);
    readImage(fname, image);
    for (int i = 2; i < 16; i = i * 2) {
        ImageType sampledImage(256/i, 256/i, 255);
        char name[] = "in.pgm";
        name[0] = '0' + i;
        name[1] = fname[0];
        for (int j = 0; j < 256/i; j++){
            for (int k = 0; k < 256/i; k++){
                int temp;
                image.getPixelVal(j*i, k*i, temp);
                sampledImage.setPixelVal(j, k, temp);
            }
        }
        ImageType scaledImage(256, 256, 255);
        for(int j = 0; j < 256; j++){
            for(int k = 0; k < 256; k++){
                int temp;
                sampledImage.getPixelVal(j/i, k/i, temp);
                scaledImage.setPixelVal(j, k, temp);
            }
        }
        writeImage(name,scaledImage);
    }
}