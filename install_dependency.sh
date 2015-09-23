#!/bin/bash
if [ $# -ne 1 ];then
	echo "Usage: ./install_dependcy.sh <temp file dir>"
	exit 1
fi
cd $1
git clone https://github.com/Itseez/opencv.git opencv
git clone https://github.com/FFmpeg/FFmpeg.git ffmpeg
git clone git://git.videolan.org/x264.git x264

cd x264
./configure --prefix=/usr/local --enable-shared 
make -j12
sudo make install

cd ../ffmpeg
./configure --prefix=/usr/local --enable-shared --enable-libx264 --enable-gpl
make -j12
sudo make install

cd ../opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_EXAMPLES=ON -D WITH_FFMPEG=ON ..
make -j12
sudo make install
