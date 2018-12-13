#include "filter.h"

#include <stdio.h>
#include <stdlib.h>

#include "image.h"

#include "logging.h"

//filter initializer
Filter::Filter()
{
    img1 = 0;
    img2 = 0;
    img.setSource(this);
}

//call update function
void Filter::Update()
{
    char msg[128];

    //update source image1
    if (img1)
    {
        //log event
        sprintf(msg, "%s: about to update input1", SourceName());
        Logger::LogEvent(msg);

        //update img1
        img1->Update();

        //log event
        sprintf(msg, "%s: done updating input1", SourceName());
        Logger::LogEvent(msg);
    }

    //update source image1
    if (img2)
    {
        //log event
        sprintf(msg, "%s: about to update input2", SourceName());
        Logger::LogEvent(msg);

        //update img2
        img2->Update();

        //log event
        sprintf(msg, "%s: done updating input2", SourceName());
        Logger::LogEvent(msg);
    }

    //log event
    sprintf(msg, "%s: about to execute", SourceName());
    Logger::LogEvent(msg);

    //execute current filter operation
    Execute();

    //log event
    sprintf(msg, "%s: done executing", SourceName());
    Logger::LogEvent(msg);
}

//execute shrinker
void Shrinker::Execute()
{
    int x, y, n1, n2;
    Pixel *output, *input;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int org_width = img1->getWidth();
    int width = img1->getWidth() / 2;
    int height = img1->getHeight() / 2;

    //initialize image
    this->img.initialize(width, height);

    //buffers
    input = img1->getBuffer();
    output = img.getBuffer();

    //copy data
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //output[y][x] = input[y*2][x*2]
            n1 = (y*width) + x;
            n2 = (y*2*org_width) + (x*2);

            //copy pixel info
            output[n1] = input[n2];
        }
    }
}

void LRCombine::Execute()
{
    int x, y, n1, n2;
    Pixel *output, *input_l, *input_r;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //check input
    if (!img2)
    {
        char msg[128];
        sprintf(msg, "%s: no input2!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->getWidth() + img2->getWidth();
    int height = img1->getHeight();

    if (img1->getHeight() != img2->getHeight())
    {
        char msg[128];
        sprintf(msg, "%s: heights must match: %d, %d", SourceName(), img1->getHeight(), img2->getHeight());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //initialize image
    this->img.initialize(width, height);

    //buffers
    input_l = img1->getBuffer();
    input_r = img2->getBuffer();
    output = img.getBuffer();

    //combine images (left+right)
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //pixel in output image
            n1 = (y * width) + x;

            //check what pixel we have and copy pixel info
	    //get pixels row-wise from left image, then start picking from right image
            if (x >= img1->getWidth())
            {
                n2 = (y * img2->getWidth()) + x - img1->getWidth();
                output[n1] = input_r[n2];
            }
            else
            {
                n2 = (y * img1->getWidth()) + x;
                output[n1] = input_l[n2];
            }
        }
    }
}

void TBCombine::Execute()
{
    int x, y, n1, n2;
    Pixel *output, *input_t, *input_b;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //check input
    if (!img2)
    {
        char msg[128];
        sprintf(msg, "%s: no input2!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->getWidth();
    int height = img1->getHeight() + img2->getHeight();

    if (img1->getWidth() != img2->getWidth())
    {
        char msg[128];
        sprintf(msg, "%s: widths must match: %d, %d", SourceName(), img1->getWidth(), img2->getWidth());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //initialize image
    this->img.initialize(width, height);

    //buffers
    input_t = img1->getBuffer();
    input_b = img2->getBuffer();
    output = img.getBuffer();

    //combine images (top+bottom)
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //pixel in output image
            n1 = (y * width) + x;

	    //check what pixel we have and copy pixel info
	    //get pixels column-wise from top image, then start picking from bottom image
            if (y >= img1->getHeight())
            {
                n2 = ((y - img1->getHeight()) * img2->getWidth()) + x;
                output[n1] = input_b[n2];
            }
            else
            {
                n2 = (y * img1->getWidth()) + x;
                output[n1] = input_t[n2];
            }
        }
    }
}

//set blend factor
void Blender::SetFactor(double f)
{
    this->factor = f;
}

//blend pixels with given factor
static void blend_pixel(Pixel *p1, Pixel *p2, double factor, Pixel *output)
{
    //blend_color = (color_1) * factor + (color_2) * factor_2
    double factor2 = 1. - factor;
    output->R = (int)(p1->R * factor + p2->R * factor2);
    output->G = (int)(p1->G * factor + p2->G * factor2);
    output->B = (int)(p1->B * factor + p2->B * factor2);
}

void Blender::Execute()
{
    int x, y, n;
    Pixel *input1, *input2, *output;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //check input
    if (!img2)
    {
        char msg[128];
        sprintf(msg, "%s: no input2!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->getWidth();
    int height = img1->getHeight();

    if (img1->getWidth() != img2->getWidth())
    {
        char msg[128];
        sprintf(msg, "%s: widths must match: %d, %d", SourceName(), img1->getWidth(), img2->getWidth());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    if (img1->getHeight() != img2->getHeight())
    {
        char msg[128];
        sprintf(msg, "%s: heights must match: %d, %d", SourceName(), img1->getHeight(), img2->getHeight());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    if (factor > 1.0)
    {
        char msg[128];
        sprintf(msg, "Invalid factor for Blender: %.6lf", factor);
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //initialize image
    this->img.initialize(width, height);

    //buffers
    input1 = img1->getBuffer();
    input2 = img2->getBuffer();
    output = img.getBuffer();

    //blend images
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //pixel in output image
            n = (y * width) + x;

            //blend
            blend_pixel(input1 + n, input2 + n, factor, output + n);
        }
    }
}

void Mirror::Execute()
{
    int x, y, n1, n2;
    Pixel *output, *input;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->getWidth();
    int height = img1->getHeight();

    //initialize image
    this->img.initialize(width, height);

    //buffers
    input = img1->getBuffer();
    output = img.getBuffer();

    //copy data
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //output[y][x] = input[y][width - x - 1]
            n1 = (y*width) + x;
            n2 = (y*width) + (width - x - 1);

            //copy pixel info
            output[n1] = input[n2];
        }
    }
}

void Rotate::Execute()
{
    int x, y, n1, n2;
    Pixel *output, *input;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->getWidth();
    int height = img1->getHeight();

    //initialize image
    this->img.initialize(height, width);

    //buffers
    input = img1->getBuffer();
    output = img.getBuffer();

    //copy data
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //output[x][height - 1 - y] = input[y][x]
            n1 = (y*width) + x;
            n2 = (x*height) + (height - y - 1);

            //copy pixel info
            output[n2] = input[n1];
        }
    }
}

//subtract pixels
static void subtract_pixel(Pixel *p1, Pixel *p2, Pixel *output)
{
    //subtract_pixel = max(color_1 - color_2, 0)
    output->R = (p1->R >= p2->R) ? (p1->R - p2->R) : 0;
    output->G = (p1->G >= p2->G) ? (p1->G - p2->G) : 0;
    output->B = (p1->B >= p2->B) ? (p1->B - p2->B) : 0;
}

void Subtract::Execute()
{
    int x, y, n;
    Pixel *input1, *input2, *output;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //check input
    if (!img2)
    {
        char msg[128];
        sprintf(msg, "%s: no input2!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->getWidth();
    int height = img1->getHeight();

    //initialize image
    this->img.initialize(width, height);

    //buffers
    input1 = img1->getBuffer();
    input2 = img2->getBuffer();
    output = img.getBuffer();

    //subtract images
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //pixel in output image
            n = (y * width) + x;

            //subtract
            subtract_pixel(input1 + n, input2 + n, output + n);
        }
    }
}

void Grayscale::Execute()
{
    int x, y, n;
    Pixel *output, *input;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->getWidth();
    int height = img1->getHeight();

    //initialize image
    this->img.initialize(width, height);

    //buffers
    input = img1->getBuffer();
    output = img.getBuffer();

    //copy data
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //output[y][x] = input[y][width - x - 1]
            n = (y*width) + x;

            //copy pixel info
            int data = input[n].R/5 + input[n].G/2 + input[n].B/4;
            output[n].R = data;
            output[n].G = data;
            output[n].B = data;
        }
    }
}

void Blur::Execute()
{
    int x, y, n;
    Pixel *input, *output;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->getWidth();
    int height = img1->getHeight();

    //initialize image
    this->img.initialize(width, height);

    //buffers
    input = img1->getBuffer();
    output = img.getBuffer();

    //blur images
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            //pixel in output image
            n = (y * width) + x;

            //blur

            //check if pixel is next to boundry then simply copy old value
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
                output[n] = input[n];
            else
            {
                //else, output = avgerage of all the neighbour colours (for all the three channels)
                output[n].R = input[n - 1].R/8 + input[n + 1].R/8 +
                              input[n - width - 1].R/8 + input[n - width].R/8 + input[n - width + 1].R/8 +
                              input[n + width - 1].R/8 + input[n + width].R/8 + input[n + width + 1].R/8;

                output[n].G = input[n - 1].G/8 + input[n + 1].G/8 +
                              input[n - width - 1].G/8 + input[n - width].G/8 + input[n - width + 1].G/8 +
                              input[n + width - 1].G/8 + input[n + width].G/8 + input[n + width + 1].G/8;

                output[n].B = input[n - 1].B/8 + input[n + 1].B/8 +
                              input[n - width - 1].B/8 + input[n - width].B/8 + input[n - width + 1].B/8 +
                              input[n + width - 1].B/8 + input[n + width].B/8 + input[n + width + 1].B/8;
            }
        }
    }
}

void Color::Execute()
{
    Pixel *output;

    //initialize image
    this->img.initialize(_width, _height);

    //buffers
    output = img.getBuffer();

    //color image
    for (int n = 0; n < _height * _width; n++)
    {
        output[n].R = _r;
        output[n].G = _g;
        output[n].B = _b;
    }
}


void CheckSum::OutputCheckSum(const char* file)
{
    FILE *f_in;
    Pixel *input;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width, height;
    unsigned char r = 0, g = 0, b = 0;

    //buffers
    input = img1->getBuffer();

    //get image params
    width = img1->getWidth();
    height = img1->getHeight();

    //calculate checksum
    for (int n = 0; n < width * height; n++)
    {
        r += input[n].R;
        g += input[n].G;
        b += input[n].B;
    }

    //read file with read binary mode
    f_in = fopen(file, "w");

    //write checksum
    fprintf(f_in, "CHECKSUM: %d, %d, %d\n", r, g, b);

    //close file
    fclose(f_in);
}
