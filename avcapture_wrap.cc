#include "avcapture_wrap.h"
#include "avcapture.h"

using namespace std;

Nan::Persistent<Function> AvCaptureWrap::constructor;

void AvCaptureWrap::Init(Local<Object> target) {
    Nan::HandleScope scope;

    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(New);
    ctor->SetClassName(Nan::New("AvCapture").ToLocalChecked());
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(ctor, "init", init);
    Nan::SetPrototypeMethod(ctor, "startCapture", startcapture);
//    Nan::SetPrototypeMethod(ctor, "lastFrame", lastframe);
    Nan::SetPrototypeMethod(ctor, "showLastFrame", showlastframe);
    
    constructor.Reset(ctor->GetFunction());
    target->Set(Nan::New("AvCapture").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(AvCaptureWrap::New) {
    Nan::HandleScope scope;

    if (info.This()->InternalFieldCount() == 0)
        return Nan::ThrowTypeError("Cannot Instantiate without new");
    AvCaptureWrap *v = new AvCaptureWrap(info[0]->NumberValue(),info[0]->BooleanValue());
    
    v->Wrap(info.This());
    
    info.GetReturnValue().Set(info.This());
}

AvCaptureWrap::AvCaptureWrap(int id, bool debug) {
    Nan::HandleScope scope;
    cam = AvCapture(id, debug);
    return;
}

NAN_METHOD(AvCaptureWrap::init) {
    Nan::HandleScope scope;
    AvCaptureWrap *v = Nan::ObjectWrap::Unwrap<AvCaptureWrap>(info.This());
    if (info.Length() <2)
        return;
    string url = std::string(*Nan::Utf8String(info[0]->ToString()));
    int ok = v->cam.init(url, info[1]->IntegerValue());
    if (ok != 0)
        Nan::ThrowError("Camera init fail\n");
    else if(info.Length() == 3) {
        v8::Local<v8::Function> cb = info[2].As<v8::Function>();
        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, 0, NULL);
    }
    info.GetReturnValue().Set(ok);
}

NAN_METHOD(AvCaptureWrap::startcapture) {
    Nan::HandleScope scope;
    AvCaptureWrap *v = Nan::ObjectWrap::Unwrap<AvCaptureWrap>(info.This());
    v->cam.startcapture();
    if (info.Length() >= 1)
    {
        v8::Local<v8::Function> cb = info[0].As<v8::Function>();
        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, 0, NULL);
    }
    return;
}

NAN_METHOD(AvCaptureWrap::showlastframe) {
    Nan::HandleScope scope;
    AvCaptureWrap *v = Nan::ObjectWrap::Unwrap<AvCaptureWrap>(info.This());
    if (info.Length() !=1)
        return;
    v->cam.showlastframe(info[0]->IntegerValue());
    return;
}

void InitAll(v8::Local<v8::Object> exports) {
    AvCaptureWrap::Init(exports);
}
NODE_MODULE(avcapture, InitAll);