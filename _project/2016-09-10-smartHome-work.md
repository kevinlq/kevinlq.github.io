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
<img src="/res/img/project/smart_home_work_hose.jpg" width="80%" height="80%" />
</center>

web监测页面:

<center>
<img src="/res/img/project/smart_home_work_server.png" width="80%" height="80%" />
</center>

主页面:
<center>
<img src="/res/img/project/smart_home_work_server2.png" width="80%" height="80%" />
</center>

## 相关技术

### zigbee自组网
本次开发中并没有使用Z-Stack协议栈，而是使用自己开发的一套微型操作系统，自定义协议。
针对2530特有的8051结构定制的一款性能优越，数据传输稳定，开发快捷的操作系统。

以下是本套系统框架使用简单截图：

<center>
<img src="/res/img/project/smart-homeWork/zigbee.png" width="80%" height="80%" />
</center>

- 自定义通信协议；
- 节点状态吗自定义；
- 自定义传感器高、低报警阈值；
- 定义订阅和取消订阅(实现传感器阈值是否自动上报)；
- 控制命令简单；

......

