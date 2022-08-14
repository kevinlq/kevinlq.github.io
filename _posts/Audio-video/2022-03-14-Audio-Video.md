---
layout: post
title: FFmpeg 学习笔记
categories: [Qt]
description: FFmpeg 常用使用笔记总结
keywords: FFmpeg，音视频
---


## 下载安装配置环境

### 下载

[下载地址](https://www.ffmpeg.org/download.html)

`Windows` 有两个版本

- Windows builds from gyan.dev
- Windows builds by BtbN

![](https://gitee.com/devstone/imageBed/raw/master/images/202203172253203.png)

### 安装

直接下载的可执行程序，不需要安装直接解压到某个文件夹，将该路径配置到系统环境变量即可

### 环境变量配置


---

## 常用命令

命令行参数有很多，基本可以分为下面 5 部分内容

```
ffmpeg {1} {2} -i {3} {4} {5}
```

- 全局参数
- 输入文件参数
- 输入文件
- 输出文件参数
- 输出文件

### 基本命令示例

- 查看支持的容器(类型)

```
ffmpeg -formats
```

- 查看视频编码格式
```
ffmpeg -codecs
```

- 查看视频编码器

```
ffmpeg -encoders
```

## 比特率

比特率又叫码率，用来表示每秒初始的字节数，单位是 `kbps` (注意是小写的 k )

- K 字节数
- b 比特(bit)
- p 每(per)
- s 秒(secend)

比特率又一个粗略的计算公式: 文件大小/时长

比如一个文件大为 12MB，时长为 88秒，那么按照粗略计算公式，比特率大概为:

```
(12 * 1024 * 8) / 88 =  1117.09 kbps
```

实际上比特率又分为三种：`VBR`, `ABR`, `CBR`

- `VBR`(Variable Bitrate) 动态比特率，数据压缩时根据音频数据实际特点实时确定采用那种采样率，这种方式压缩质量非常高
- `ABR`(Average Bitrate) 平均比特率，这种方式是 `VBR`的一种平均值(或者叫插值参数)
- `CBR`(Constant Bitrate) 常数比特率，文件从头到尾比特率不变化



## 参考文章

- [官方](https://ffmpeg.org/ffmpeg.html)
- [ruanyifeng](https://www.ruanyifeng.com/blog/2020/01/ffmpeg.html)
- [自动计算码率](https://toolstud.io/video/bitrate.php?imagewidth=1920&imageheight=1080&colordepth=24&framerate=60)

******

    作者:鹅卵石
    时间: 2021年3月10
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->



---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="https://gitee.com/devstone/imageBed/raw/master/code/myCode.png" width="50%" height="50%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.