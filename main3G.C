#include <PNMreader.h>
#include <PNMwriter.h>
#include <filter.h>

int main(int argc, char *argv[])
{
    PNMreader reader(argv[1]);

    Shrinker sr;
    LRCombine lr1;
    LRCombine lr2;
    LRCombine lr3;
    LRCombine lr4;
    LRCombine lr5;
    LRCombine lr6;
    TBCombine tb1;
    TBCombine tb2;
    TBCombine tb3;
    Blur br1;
    Blur br2;
    Blur br3;
    Subtract sb;
    Rotate rt1;
    Rotate rt2;
    Rotate rt3;
    Rotate rt4;
    Mirror mr;
    Grayscale gs;

    Color green(893, 672, 10, 64, 40);
    Color gold(893, 672, 255, 215, 0);

    br1.SetInput(reader.GetOutput());
    sr.SetInput(br1.GetOutput());

    rt1.SetInput(sr.GetOutput());
    rt2.SetInput(rt1.GetOutput());
    br2.SetInput(rt2.GetOutput());
    rt3.SetInput(br2.GetOutput());
    rt4.SetInput(rt3.GetOutput());
    br3.SetInput(rt4.GetOutput());

    lr1.SetInput(sr.GetOutput());
    lr1.SetInput2(rt2.GetOutput());
    lr2.SetInput(br2.GetOutput());
    lr2.SetInput2(br3.GetOutput());
    tb1.SetInput(lr1.GetOutput());
    tb1.SetInput2(lr2.GetOutput());

    sb.SetInput(br1.GetOutput());
    sb.SetInput2(reader.GetOutput());

    lr3.SetInput(tb1.GetOutput());
    lr3.SetInput2(sb.GetOutput());

    mr.SetInput(sb.GetOutput());

    gs.SetInput(sr.GetOutput());

    lr4.SetInput(gs.GetOutput());
    lr4.SetInput2(green.GetOutput());
    lr5.SetInput(gold.GetOutput());
    lr5.SetInput2(gs.GetOutput());
    tb2.SetInput(lr4.GetOutput());
    tb2.SetInput2(lr5.GetOutput());

    lr6.SetInput(mr.GetOutput());
    lr6.SetInput2(tb2.GetOutput());

    tb3.SetInput(lr3.GetOutput());
    tb3.SetInput2(lr6.GetOutput());


    tb3.GetOutput()->Update();

    PNMwriter writer;
    writer.SetInput(tb3.GetOutput());
    writer.Write(argv[2]);

    CheckSum cs;
    cs.SetInput(tb3.GetOutput());
    cs.OutputCheckSum("proj3G_checksum");
}
