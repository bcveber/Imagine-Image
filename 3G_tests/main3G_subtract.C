#include <PNMreader.h>
#include <PNMwriter.h>
#include <filter.h>

int main(int argc, char *argv[])
{

    Color c1(100, 100, 10, 20, 30);
    Color c2(100, 100, 255, 155, 55);
    Color c3(200, 100, 50, 50, 50);

    LRCombine lr;
    lr.SetInput(c1.GetOutput());
    lr.SetInput2(c2.GetOutput());

    Subtract single_filter;

    single_filter.SetInput(lr.GetOutput());
    single_filter.SetInput2(c3.GetOutput());

    single_filter.GetOutput()->Update();

    PNMwriter writer;
    writer.SetInput(single_filter.GetOutput());
    writer.Write(argv[2]);

    CheckSum cs;
    cs.SetInput(single_filter.GetOutput());
    cs.OutputCheckSum("subtract_checksum");
}
