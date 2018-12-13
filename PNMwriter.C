#include "PNMwriter.h"

#include <stdio.h>
#include <stdlib.h>

//write image
void PNMwriter::Write(char *file)
{
    FILE *f_in;
    int width, height, maxval;

    //get image params
    width = img1->getWidth();
    height = img1->getHeight();
    maxval = img1->getMaxVal();

    //read file with read binary mode
    f_in = fopen(file, "wb");

    //write image header
    fprintf(f_in, "%s\n%d %d\n%d\n", "P6", width, height, maxval);

    //write the elements
    fwrite(img1->getBuffer(), 3, (width * height), f_in);

    //close file
    fclose(f_in);
}
