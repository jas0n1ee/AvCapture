AvCapture
=====
I'm recently using OpenCV to do some video processing. And when dealing with some rtsp stream from a IP camera, I found the `VideoCapture` in OpenCV can't do it in tcp way and some time have a huge delay. 
Then I turned into libavformat, which is part of ffmpeg.
To use AvCapture, you need install libavformat and OpenCV(as I'm using `Mat` to store video frames). I strongly suggest you install libx264 as well for h264 decoding. These dependency installation can either be done by some apt-get/yum/pacman/brew command or using a script provided in this project.

### Some description
AvCapture will automatically create a thread to receive and decode the stream. This might give you extra diffculty to debug.  
