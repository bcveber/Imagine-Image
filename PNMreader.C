#include "PNMreader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"

PNMreader::PNMreader(char* path)
{
    //copy file path in new array
    this->file = new char[strlen(path) + 1];
    strcpy(this->file, path);

    //set image source
    img.setSource(this);
}

void PNMreader::Update()
{
    char msg[128];

    //log event
    sprintf(msg, "%s: about to execute", SourceName());
    Logger::LogEvent(msg);

    //execute PNMreader
    Execute();

    //log event
    sprintf(msg, "%s: done executing", SourceName());
    Logger::LogEvent(msg);
}

void PNMreader::Execute()
{
    FILE *f_in;
    int width, height, maxval;
    char magicNum[128];

    //read file with read binary mode
    f_in = fopen(this->file, "rb");

    //check if file was opened successfully
    if (!f_in)
    {
        char msg[128];
        sprintf(msg, "%s: file not found!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //read header of image file
    fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);

    //only work with P6 format currently
    if (strcmp(magicNum, "P6"))
        return;

    //initialize image
    this->img.initialize(width, height);

    //read pixels
    //height * width = num of structs to read
    fread(this->img.getBuffer(), 3, width * height, f_in);

    //close file
    fclose(f_in);
}
