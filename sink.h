#ifndef SINK_H
#define SINK_H

#include "image.h"

class Sink
{
protected:
    
Image *img1, *img2;
public:
    void SetInput(Image *img);		//set first input image
    void SetInput2(Image *img);		//set second image
};

#endif
