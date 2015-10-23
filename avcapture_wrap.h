#include "avcapture.h"
#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <node_version.h>
#include <node_buffer.h>
#include <nan.h>

using namespace v8;
using namespace node;

class AvCaptureWrap: public Nan::ObjectWrap {
public:
    AvCapture cam;

    static Nan::Persistent<Function> constructor;
    static void Init(Local<Object> target);
    static NAN_METHOD(New);

    AvCaptureWrap(int id=0, bool debug = false);

    static NAN_METHOD(init);
    static NAN_METHOD(startcapture);
//    static NAN_METHOD(lastframe);
    static NAN_METHOD(showlastframe);
};

