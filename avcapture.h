#ifndef AVCAPTURE_H
#define AVCAPTURE_H
#include "opencv2/opencv.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/video/background_segm.hpp"
#include <vector>
#include <pthread.h>
#include <algorithm>
extern "C" {
#include <libavformat/avformat.h>
}
#endif

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
    AvCapture(int id=0, bool debug = false);
    ~AvCapture();
    int init(string url, int buffer_size);
    void startcapture();
    bool lastframe(Mat &);
    void showlastframe(int);
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