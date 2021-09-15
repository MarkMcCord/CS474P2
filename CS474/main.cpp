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
    char lenna[] = "lenna.pgm";
    char peppers[] = "peppers.pgm";
    sample(lenna);
    sample(peppers);

    //part 3
    char boat[] = "lenna.pgm";
    char f_16[] = "f_16.pgm";
    eqHistogram(boat);
    eqHistogram(f_16);

    return 0;
}

void eqHistogram(char fname[])
{
    ImageType image(256, 256, 255);
    readImage(fname, image);

    //Declaration for storing the two histograms
    vector<int> hist(256, 0);
    vector<int> eqhist(256, 0);

    //File for output
    char name[] = "xxx.pgm";
    name[0] = 'e';
    name[1] = 'q';
    name[2] = fname[0];

    //Calculate Histogram
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 256; j++){
            int temp;
            image.getPixelVal(i, j, temp);
            hist[temp]++;
        }
    }

    //Calculate total amount of pixels
    int total = 256 * 256;
    int curr = 0;

    // calculating cumulative frequency and new gray levels
    for (int i = 0; i < 256; i++) {
        // cumulative frequency1
        curr += hist[i];
  
        // calculating new gray level after multiplying by
        // maximum gray count which is 255 and dividing by
        // total number of pixels
        eqhist[i] = round((((float)curr) * 255) / total);
    }

    ImageType eqImage(256, 256, 255);

    // performing histogram equalisation by mapping new gray levels
    for (int i = 0; i < 256; i++) {
        for(int j = 0; j < 256; j++){
            // mapping to new gray level values
            int temp, temp2;
            image.getPixelVal(i, j, temp);
            eqImage.setPixelVal(i, j, eqhist[temp]);
        }
    }

    //Output equalized image
    writeImage(name, eqImage);

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