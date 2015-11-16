#ifndef AVCAPTURE_H
#define AVCAPTURE_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <pthread.h>
#include <algorithm>
extern "C" {
#include <libavformat/avformat.h>
}

using namespace std;
using namespace cv;

struct state {
    AVFormatContext *avformat_ctx = NULL;
    AVPacket pkt;
    AVCodecContext *codec_ctx;
    AVFrame *frame;
    int best_stream;
    cv::Mat y;
    cv::Mat u;
    cv::Mat v;
    cv::Mat rgb;
    pthread_mutex_t *buff_mutex;
    pthread_cond_t *cond_camready;
    vector<Mat *> *buffer;
    int buffer_size;
    int id;
    int* fcnt;
    bool debug = false;
};

class AvCapture
{
public:
    /*  Create object with a uniq id and debug mode.
        With debug mode on, avcapture will auto 
        print cam id and frame count on cli */
    AvCapture(int id=0, bool debug = false);
    ~AvCapture();
    /*  Init object with rtsp url and buffer_size.
        Everytime you lunch a lastframe(Mat &) func
        the first img in buffer will pop out. And when
        buffer is full it will remove the oldest frame.
        When this func return 0, it's ready to start 
        capture*/
    int init(string url, int buffer_size);
    /*  Only lunch startcapture() after lunching 
        init(string, int) func with a return value 0.
        It will create a thread to capture in background*/
    void startcapture();
    /*  Run this func to catch up with the first img
        in the buffer */
    bool lastframe(Mat &);
    /*  Run this func to show the latest img */
    void showlastframe(int);
    void stop();
    int id;
    int fcnt=0;
    bool debug=false;
    friend void * cam_read(void *arg);
    pthread_mutex_t buff_mutex;
    pthread_cond_t cond_camready;
private:
    struct state info;
    vector<Mat*> buffer;
    pthread_t thread_id;

};
#endif
