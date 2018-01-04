---
layout: post
title: DICOM格式学习
categories: 医学影像
description: DICOM是医学数字成像和通信的标准
keywords: DICOM，通信协议
---

## DICOM简介
`DICOM`即 `Digital Imaging and Communications in Medicine`，医学数字成像和通信，是医学图像和相关信息的国际标准。它定义了满足临床需求的可用于数据交换的医学图像格式。

## DICOM文件结构

![dicom文件结构](/res/img/blog/medical_image/dicom_style.jpg)

![dicom文件结构](/res/img/blog/medical_image/dicom_data_con.png)

开始时128字节的导言部分(通俗的说就是没有啥意义的破数据)，接着会是4字节的DICOM文件标识，存储这个`DICM`，然后就是`dicom`数据元素了.

## DICOM文件解读

![dicom文件结构](/res/img/blog/medical_image/dicom_con.png)

使用任意一个编辑器打开某个.dcm文件，就会看到上面的界面。

红色标注的就是字节码，前面8行是文件导言部分，没有用，从第9行开始，`44 49 43 4D`这四个数字很重要，转化成ASCII码就是`DICM`,表示了一个DICOM文件，如果这四个16进制数据丢失或者损坏，就无法打开该图像了。


## 参考文章
[参考博文](http://blog.csdn.net/zssureqh/article/details/41016091)
[cnblog解读](http://www.cnblogs.com/assassinx/archive/2013/01/09/dicomViewer.html)