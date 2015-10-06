#include "avcapture.h"

void * cam_read(void *arg) {
    int ret;
    int frame_finished;
    state *cam = (struct state *)arg;
    int frame_cnt = 0;
    int fall_cnt = 0;
    while (1)
    {
        av_frame_unref(cam->frame);
        while (((ret = av_read_frame(cam->avformat_ctx, &(cam->pkt))) == 0) && (cam->pkt.stream_index == cam->best_stream)) {
            ret = avcodec_decode_video2(cam->codec_ctx, cam->frame, &frame_finished, &(cam->pkt));
            av_packet_unref(&(cam->pkt));
            if (frame_finished) { // decoded a complete frame, put it in an openv mat
                cam->y = cv::Mat(cam->frame->height, cam->frame->width, CV_8UC1, cam->frame->data[0]);
                cam->u = cv::Mat(cam->frame->height/2, cam->frame->width/2, CV_8UC1,cam->frame->data[1]);
                cam->v = cv::Mat(cam->frame->height/2, cam->frame->width/2, CV_8UC1,cam->frame->data[2]);
                Mat yuv(cam->y.rows*3/2,cam->y.cols,CV_8UC1);
                memcpy(yuv.data,cam->frame->data[0],cam->y.cols*cam->y.rows*sizeof(uchar));
                memcpy(yuv.data+cam->y.cols*cam->y.rows,cam->frame->data[1],cam->y.cols*cam->y.rows*sizeof(uchar)/4);
                memcpy(yuv.data+cam->y.cols*cam->y.rows*5/4,cam->frame->data[2],cam->y.cols*cam->y.rows*sizeof(uchar)/4);
                cvtColor(yuv,cam->rgb,CV_YUV2BGR_I420);
                //TODO: Check whether rgb's address changes each time
                pthread_mutex_lock(cam->buff_mutex);
                cam->buffer->push_back(&(cam->rgb));
                if (cam->buffer->size() > cam->buffer_size)
                    cam->buffer->erase(cam->buffer->begin());
                cam->fcnt ++;
                pthread_mutex_unlock(cam->buff_mutex);
                pthread_cond_signal(cam->cond_camready);
                frame_cnt ++;
                if (cam->debug)
                    cout <<"process "<<cam->id<<" read finished :"<<frame_cnt<<endl;
            }
        }
        if (!frame_finished)
        {
            cerr<<"ERROR"<<cam->id<<": Decode frame not complete\n";
            fall_cnt ++;
        }
        else (fall_cnt > 1)?fall_cnt -- : fall_cnt = 0;
        if (fall_cnt > 20) break;
    }
    pthread_exit(NULL);
}
AvCapture::AvCapture(int id, bool debug)
{
    this->id = id;
    this->debug = debug;
    pthread_mutex_init(&buff_mutex, NULL);
    pthread_cond_init(&cond_camready, NULL);
}
AvCapture::~AvCapture()
{
    buffer.clear();
}
int AvCapture::init(string url, int buffer_size)
{
    av_register_all();
    avformat_network_init();
    AVDictionary *avdict = NULL;
    AVCodec *codec;
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    av_dict_set(&avdict, "rtsp_transport", "tcp", 0);
    info.codec_ctx = avcodec_alloc_context3(codec);
    avcodec_open2(info.codec_ctx,codec,0);
    int ret = avformat_open_input(&(info.avformat_ctx), url.c_str(), NULL, &avdict);
    info.best_stream = av_find_best_stream(info.avformat_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    av_init_packet(&(info.pkt));
    info.frame = av_frame_alloc();
    

    info.buffer = &buffer;
    info.buffer_size = buffer_size;
    info.buff_mutex = &buff_mutex;
    info.cond_camready = &cond_camready;
    info.id = id;
    info.debug = debug;
    info.fcnt = &this->fcnt;
    return ret;
}

bool AvCapture::lastframe(Mat &dst)
{
    pthread_mutex_lock(&buff_mutex);
    while (buffer.size() == 0)
        pthread_cond_wait(&cond_camready, &buff_mutex);
    Mat *last = buffer.back();
    last->copyTo(dst);
    buffer.pop_back();
    pthread_mutex_unlock(&buff_mutex);
    
    return true;
}

void AvCapture::startcapture()
{
    pthread_create(&thread_id, NULL, cam_read, &info);
}

void AvCapture::showlastframe(int cnt = -1)
{
    Mat pic;
    for (int i = cnt; i!=0; i--)
    {
        lastframe(pic);
        imshow("Capture", pic);
        cvWaitKey(1);
    }
}
