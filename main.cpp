#include <iostream>
#include "avcapture.h"

using namespace std;

int main()
{
    AvCapture sample(1);
    if (sample.init("rtsp://admin:cisco123@171.68.72.72", 10) == 0)
        sample.startcapture();
    else exit(0);
    while (1)
    {
        Mat lst_frame;
        sample.lastframe(lst_frame);
        imshow("Video0",lst_frame);
        cvWaitKey(1);
    }
}