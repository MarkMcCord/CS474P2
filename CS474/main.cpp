#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include "image.h"

using namespace std;

void readImage(char fname[], ImageType& image);
void writeImage(char fname[], ImageType& image);
void sample (char fname[]);

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

    return 0;
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