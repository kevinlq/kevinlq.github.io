---
layout: post
title: QImage学习学习
categories: Qt学习
description: QImage相关操作
keywords: QImage,位深
---

QImage这个类之前用过，无外乎是加载一个图片文件显示出来，并没有做过多的研究，目前工作中用到了灰度图以及图片的像素操作，重新学习了下，记录记录。

## 一些基本操作方法

- 获取图像的首地址：
```C++
const uchar *QImage::bits() const
```

- 获取图像的总字节数
```C++
int QImage::byteCount() const
```

- 获取图像每行字节数
```C++
int QImage::bytesPerLine() const  
还可以这样计算(width:图像宽度，img.depth是图图像深度):
int bytePerLine = (width * img.depth()  +  31) / 32 * 4;
```

- 存入图像，格式为R,G,B,A(0,1,2,3)
```C++
QImage::Format_RGB32
```

- 存入图像，格式为R,G,B(0,1,2)
```C++
QImage::Format_RGB888
```

- 存入图像，此时需要设定一张颜色表`QVector<QRgb>`,如下
```C++
QImage::Format_Indexed8
```

灰度颜色表：
```C++
QVector<QRgb> vtrColor;
for(int k = 0;k < 256;++k)
{
    vtrColor.push_back( qRgb(k,k,k) );
}

```

## 相关概念

### 灰度图


### 位深
位深是指存储每个像素所用的位数.
