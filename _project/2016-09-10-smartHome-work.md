---
layout: project
title: 智能家居(实际项目)
categories: [项目]
description: 真实环境下的智能家居
keywords: 智能家居,zigbee，ARM
---

这个版本的智能家居系统属于工作后在单位做的，已经实地部署使用。

>智能家居系统以zigbee节点作为数据传输单元、 以嵌入式网关为交互中间件， 通过物联港
实现对家居设备交互和控制。产品形态有系统网关设备， zigbee 节点设备，传感器若干

![](/res/img/project/smart_home_work.png)

> 所有的数据节点以及控制节点都是由zigbee终端节点进行控制，首先都和zigbee协调器交互，协调器最后和ARM网关进行交互，ARM网关最后和服务器之间进行交互.

## 环境说明

- 开发系统: windows7、Linux
- 硬件环境: CC2530，Exynos4412芯片方案
- 软件环境: C/C++，MySQL,IAR,Embedded Linux

## 模块节点

- 光温湿节点
- 智能插座
- RFID
- 窗磁
- 智能红外对射
- 智能窗帘
- 空调控制节点
- 电视控制节点
- 门禁控制节点
- 摄像头模块
- 噪声节点
- 火焰节点
- 声光报警节点
- 雨滴传感器节点
- 可燃气体节点
- 吊顶热释电节点
- 智能灯光节点

## 实际效果展示

<center>
<img src="/res/img/project/smart_home_work_hose.jpg" width="50%" height="50%" />
</center>

web检测页面:

<center>
<img src="/res/img/project/smart_home_work_server.png" width="50%" height="50%" />
</center>

<center>
<img src="/res/img/project/smart_home_work_server2.png" width="50%" height="50%" />
</center>