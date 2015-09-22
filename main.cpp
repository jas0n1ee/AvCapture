#include <iostream>
#include "avcapture.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout<<"Usage: ./sample rtsp://username:passwd@ip\n";
        exit(1);
    }
    AvCapture sample(1);
    if (sample.init(argv[1], 10) == 0)
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