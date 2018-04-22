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
灰度颜色表：
QVector<QRgb> vtrColor;
for(int k = 0;k < 256;++k)
{`
    vtrColor.push_back( qRgb(k,k,k) );
}
```


## 相关概念

### 灰度图
普通彩色图片中每个像素中有R、G、B三个分量，而每个分量有256种(0~255)值可以选择,这样一个像素点就可以有1600多万(255*255*255)的颜色变化范围。

而灰度图就是R、G、B三分分量的值相同的一种特殊彩色图像。即R=G=B.


### 位深
位深是指存储每个像素所用的位数.

如上面介绍的QImage方法，`img.depth()`,该方法可以返回当前图像的位深.



## 处理算法

- 常规处理

```C++
unsigned char *grayData;
    QImage img;
    img.load (m_strPath+"/2.jpg");

    QPixmap pixmap(QPixmap::fromImage (img));

    ui->label->setPixmap (pixmap);

    unsigned char *data = img.bits ();

    int w = img.width ();
    int h = img.height ();

    int bytePerLine = (w * 24 + 31) / 8;
    //存储处理后的数据
    grayData = new unsigned char [bytePerLine * h];
    unsigned char r,g,b;
    for ( int i = 0; i < h; i++ )
    {
        for ( int j = 0; j < w; j++ )
        {
            r =  *(data + 2);
            g = *(data + 1);
            b = *data;

            grayData[i * bytePerLine + j * 3] = (r * 30 + g*59 +b*11)/100;
            grayData[i*bytePerLine+j*3+1]=(r*30+g*59+b*11)/100;
            grayData[i*bytePerLine+j*3+2]=(r*30+g*59+b*11)/100;

            data+=4;

            //ui->textEdit->append (QString("%1 %2 %3").arg (r).arg (g).arg (b));
        }
    }

    QImage grayImage(grayData,w,h,bytePerLine,QImage::Format_RGB888);

    QPixmap pixmap2(QPixmap::fromImage (grayImage));

    ui->label_2->setPixmap (pixmap2);

```

![](/res/img/blog/Qt学习/image-del/1.png)
