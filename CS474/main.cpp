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

int main(int argc, char * argv[])
{
    int val;

    ImageType image(256, 256, 255);
    readImage(argv[1], image);
    image.getPixelVal(2, 2, val);
    writeImage(argv[2], image);
    return 0;
}