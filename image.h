#ifndef IMAGE_H
#define IMAGE_H

class Source;

//pixel structure
typedef struct
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Pixel;

class Image
{
    Pixel* buffer;
    Source *source;
    int width, height, maxval;
public:
    void setSource(Source *src);
    int getWidth();
    int getHeight();
    int getMaxVal();
    Pixel* getBuffer();             //get pointer to image buffer
    void initialize(int w, int h);   //set new image buffer
    void Update();
};

#endif
