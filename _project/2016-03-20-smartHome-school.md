---
layout: project
title: 智能家居(毕设版本)
categories: [项目]
description: 智能家居demo演示版本
keywords: 智能家居, STM32，zigbee，ARM
---

与其说是智能家居，不如说是智能家居demo，其实也可以直接说是环境监测系统.

[想要下载，请点击这里](https://github.com/kevinlq/SmartHome-Qt)

## 1. 开发环境

- windows7 64b,ubuntu 14.04 32b
- Qt4.8.6
- IAR:8.0
- Altium Designer 13.0
- arm-linux-gcc 4.4.3
- STM32,CC2530,ARM9

## 2. 整体流程

![](/res/img/project/smart_home/pro.png)

## 3. 主要功能

### 3.1 STM32

- 应用了小型的嵌入式操作系统μC/OS-II； 
- 各个传感器的数据以多任务的形式获取，实时性很好； 
- 数据调试使用STM32串口1； 
- 数据的发送使用STM32的串口3，并以任务的形式进行打包发送，具体数据帧格式请看代码； 
- STM32接收数据采用终端方式，当有数据到来时会触发终端，进入数据接收解析过程，在此过程中会对数据的帧头等信息进行判断

### 3.2 zigbee

Zigbee部分其实没有太多功能，一句话概括就是一个串口透传功能！本来当初计划直接使用zigbee驱动各个传感器，然后组网后发送给协调器，考虑到既然学习了 STM32, 就利用上吧！其实这里完全可以直接使用 zigbee, 不使用 STM32 的。 两套方案的编码工作其实早就已经完成了，思考了好久才决定采用 STM32 + zigbee 方式，这样下来可以学到很多，尤其是 `μC/OS-II` 系统的移植，还是很深奥的，很值的以后继续深入的去学习。 

### 3.3 ARM上位机

- 模拟室内三个场景：卧室、客厅、厨房 
- 分别对这个三个场景下的温度，湿度以及烟雾浓度进行一个测量并显示； 
- 使用LED灯模拟室内照明灯； 
- 使用人体红外传感器进行检测是否有人(这个传感器有点不靠谱，最后取消了) 
- 当初计划添加视频监控，到了后期发现工作量太大，没有时间做了，所以就没有完成.


## 4. 实际效果展示

### 4.1 上位机首页

<center>
<img src="/res/img/project/smart_home/home.png" width="70%" height="70%" />
</center>

### 4.2 控制界面

<center>
<img src="/res/img/project/smart_home/controlHome.png" width="70%" height="70%" />
</center>

### 4.3 客厅控制

<center>
<img src="/res/img/project/smart_home/parlour.png" width="70%" height="70%" />
</center>

### 4.4 历史温度曲线

<center>
<img src="/res/img/project/smart_home/temp.png" width="70%" height="70%" />
</center>

## 5实物展示

<center>
<img src="/res/img/project/smart_home/hardware.jpg" width="70%" height="70%" />
</center>

---

更加详细的介绍内容在我的 `CSDN` 博客中，当时答辩完成后，针对这demo分为五部分进行了讲解，感兴趣的可以到这里进行阅读 [请点击这里](http://blog.csdn.net/u013704336/article/details/50832978)