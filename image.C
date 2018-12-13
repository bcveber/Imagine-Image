#include "image.h"

#include "source.h"

int Image::getWidth()
{
    return this->width;
}

int Image::getHeight()
{
    return this->height;
}

int Image::getMaxVal()
{
    //pixel max value
    return this->maxval;
}

Pixel* Image::getBuffer()
{
    //buffer pointer
    return this->buffer;
}

//set new image buffer
void Image::initialize(int w, int h)
{
    this->buffer = new Pixel[w * h]; //set image buffer
    this->width = w;                 //set image width
    this->height = h;
    this->maxval = 255;
}

void Image::setSource(Source *src)
{
    this->source = src;
}

void Image::Update()
{
    if (this->source)
        this->source->Update();
}

