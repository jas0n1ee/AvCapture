#include <avcapture.h>
#include <opencv2/video/background_segm.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
    AvCapture cam(0,true);
    if (cam.init(argv[1], 10)==0)
        cam.startcapture();
    Mat img;
    Mat fMask;
    Mat bgImg;
    Mat fMask_bl;
    Mat fMask_th;
    Mat result;
    int i=0;
    int w=0, h=0;
    Ptr<BackgroundSubtractor> bs = createBackgroundSubtractorMOG2(20000,40,true);
    cvWaitKey(1000);
    while(++i) {
        cam.lastframe(img);
        resize(img,img,Size(640,360));
        img.copyTo(result);
        w = img.cols;
        h = img.rows;
        bs->apply(img,fMask);
        if (i < 100) continue;
        bs->getBackgroundImage(bgImg);
        GaussianBlur(fMask,fMask_bl,Size(0,0),4.0);
        threshold(fMask_bl,fMask_th,60,255,THRESH_BINARY);
        Mat con;
        fMask_th.copyTo(con);
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(con, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        for (int j = 0; j < contours.size(); j++)
        {
            Rect boundrect = boundingRect(contours[j]);
            rectangle(result,boundrect,Scalar(255,255,255),4);
            char cord[15];
            Point t = boundrect.tl() + Point(boundrect.width/2,boundrect.height/2);
            sprintf(cord,"x:%d y:%d",t.x,t.y);
            putText(result,cord, boundrect.tl() - Point(0,10), FONT_HERSHEY_COMPLEX_SMALL, 0.9, cvScalar(200,250,200), 1, CV_AA);
        }

        imshow("img",img);
        moveWindow("img", 0, 0);
        imshow("background",bgImg);
        moveWindow("background", w, 0);
        imshow("front mask",fMask_th);
        moveWindow("front mask", 0, h+20);
        imshow("result",result);
        moveWindow("result", w, h+20);
        cvWaitKey(50);
    }
}