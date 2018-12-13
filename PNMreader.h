#ifndef PNM_READER_H
#define PNM_READER_H

#include "source.h"

class PNMreader : public Source
{
    char *file;

  protected:
    void Execute(); //PNMreader run function

  public:
    PNMreader(char *path); //initialize reader w/ path
    void Update();
    const char* SourceName() { return "PNMreader"; } //return source name
};

#endif
