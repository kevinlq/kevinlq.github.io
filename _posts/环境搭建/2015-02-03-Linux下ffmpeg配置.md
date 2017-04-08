---
layout : life
title: Linux下ffmpeg安装与开发配置
category : 环境搭建
duoshuo: true
date : 2015-02-03
---

<!-- more -->

******

### ***ffmpeg安装***

***删除已安装的文件，避免冲突***

```sh
	sudo apt-get remove ffmpeg x264
	sudo apt-get autoremove
```

***安装需要的支持***

```sh
	sudo apt-get install make automake g++ bzip2 python unzip patch subversion ruby build-essential git-core checkinstall yasm texi2html libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libsdl1.2-dev libtheora-dev libvdpau-dev libvorbis-dev libvpx-dev libx11-dev libxfixes-dev libxvidcore-dev zlib1g-dev
```

***安装 x264 （自己选择是否安装）***

x264 依赖于yasm，而且版本较高。

```sh
	sudo git clone git://git.videolan.org/x264.git
	cd x264
	sudo ./configure --enable-shared --prefix=/usr/local
	sudo make
	sudo make install
	cd ..
```

***安装FFMPEG***

```sh
	sudo wget http://ffmpeg.org/releases/ffmpeg-2.5.3.tar.bz2
	sudo tar xvjf ffmpeg-2.5.3.tar.bz2
	cd ffmpeg-2.5.3
```

```sh
	./configure --enable-gpl --enable-version3 --enable-nonfree --enable-postproc --enable-libfaac --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libxvid --enable-shared --prefix=/usr/local
```

```sh
	sudo make
	sudo make install
	cd ..
```

configure过程根据自己的情况开启某部分功能。

***常见错误***

```sh
	ffmpeg: error while loading shared libraries: libavdevice.so.53: cannot open shared object file: No such file or directory
```

解决办法：

```sh
vi /etc/ld.so.conf
加入：/usr/local/lib
执行ldconfig
```



