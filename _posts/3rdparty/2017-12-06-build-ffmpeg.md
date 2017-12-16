---
layout: post
title: Windows下MinGW跨平台编译FFmpeg
categories: [开源库]
description: 编译ffmpeg
keywords: Qt,MinGW,FFmpeg,编解码
---

`FFmpeg` 是一个开源的,跨平台的音视频记录、转换解决方案.

## 下载

[主页](http://ffmpeg.org/)


## 模块介绍

- libavcodec: 编解码器
- libavformat:封装格式处理(对音视频格式解析)
- libavfilter:滤镜特效处理
- libavutil:工具库(FFmpeg大部分需要)
- libpostproc:后期效果处理
- libswscale:视频场景比例缩放，色彩映射转换
- libswresample:音频采样数据格式转换


## 编译

根据依赖关系，首先肯定得先编译 `libavutil` 模块

未完……