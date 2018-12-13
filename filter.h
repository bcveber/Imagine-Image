#ifndef FILTER_H
#define FILTER_H

#include "sink.h"
#include "source.h"

class Filter : public Source, public Sink
{
public:
    Filter();
    void Update();
};

class Shrinker : public Filter
{
  protected:
    void Execute(); //execute shrinker
  public:
    const char* SourceName() { return "Shrinker"; } //return source name
};

//LR concat filter class
class LRCombine : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "LRConcat"; } //return source name
};

class TBCombine : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "TBConcat"; } //return source name
};

class Blender : public Filter
{
    double factor;

protected:
    void Execute();           //execute blender

  public:
    const char* SourceName() { return "Blender"; } //return source name
    void SetFactor(double f); //set blending factor
};

class Mirror : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Mirror"; } //return source name
};

class Rotate : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Rotate"; } //return source name
};

class Subtract : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Subtract"; } //return source name
};

class Grayscale : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Grayscale"; } //return source name
};

class Blur : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Blur"; } //return source name
};

class Color : public Filter
{
  int _r, _g, _b, _width, _height;
  public:
    Color(int width, int height, int r, int g, int b) : Filter(), _width(width), _height(height), _r(r), _g(g), _b(b) {}
    const char* SourceName() { return "Color"; } //return source name
  protected:
    void Execute();
};

class CheckSum : public Sink
{
  public:
    const char* SourceName() { return "Checksum"; } //return source name
    void OutputCheckSum(const char* file); //OutputCheckSum function
};

#endif
