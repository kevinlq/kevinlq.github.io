---
layout: post
title: QPinter 常用绘制图像的方法
categories: [Qt学习]
description: 在使用 Qt 绘制图片时需要注意的事项
keywords: Qt,QPainter
---
>阅读本文大概需要 3 分钟

我们在开发软件的过程中，绘制图像功能必不可少，使用 Qt 绘制图像时非常简单，只需要传递几个参数就可以实现功能，在 Qt 中绘制图像的 `api`有好几个
```C++
    void drawImage(const QRectF &targetRect, const QImage &image, const QRectF &sourceRect,
                   Qt::ImageConversionFlags flags = Qt::AutoColor);
    inline void drawImage(const QRect &targetRect, const QImage &image, const QRect &sourceRect,
                          Qt::ImageConversionFlags flags = Qt::AutoColor);
    inline void drawImage(const QPointF &p, const QImage &image, const QRectF &sr,
                          Qt::ImageConversionFlags flags = Qt::AutoColor);
    inline void drawImage(const QPoint &p, const QImage &image, const QRect &sr,
                          Qt::ImageConversionFlags flags = Qt::AutoColor);
    inline void drawImage(const QRectF &r, const QImage &image);
    inline void drawImage(const QRect &r, const QImage &image);
    void drawImage(const QPointF &p, const QImage &image);
    inline void drawImage(const QPoint &p, const QImage &image);
    inline void drawImage(int x, int y, const QImage &image, int sx = 0, int sy = 0,
                          int sw = -1, int sh = -1, Qt::ImageConversionFlags flags = Qt::AutoColor);
```

不过最常用的还是下面这个
```
void drawImage(const QRectF &targetRect, const QImage &image, const QRectF &sourceRect,
                   Qt::ImageConversionFlags flags = Qt::AutoColor);
```

其中有几个参数需要注意并理解下

- `targetRect` 表示要绘制的目标矩形大小，也就是最终要显示的矩形大小区域
- `sourceRect` 表示源图像要绘制的区域，会根据该区域大小来绘制图像


下面是测试代码

```
    QImage image("G:/drawImage_test.png");

    int nWidth = image.width();
    int nHeight = image.height();

    QRectF target(10.0, 20.0, nWidth, nHeight);  //建立目标矩形，表示从10，20开始位置绘制图像
    QRectF source(0.0, 0.0, 256.0, 256.0);    //建立源矩形，表示从图像0，0位置开始，取256大小的区域去绘制
    QImage backImage(nWidth, nHeight, QImage::Format_ARGB32);
    backImage.fill(Qt::gray);

    QPainter painter(&backImage);
    painter.drawImage(target, image, source);
    backImage.save("G:/drawImage_test_out.png", "png");
```
运行程序，效果如下

原始图像 `drawImage_test.png`

![](https://gitee.com/devstone/imageBed/raw/master/images/20201009141300.png)


绘制后的图像 `drawImage_test_out.png`

![](https://gitee.com/devstone/imageBed/raw/master/images/20201009141335.png)

可以明显看到，绘制图像从(10,20)开始，截取原始图像(0,0)位置开始，256*256大小区域进行了绘制





******

    作者:鹅卵石
    时间: 2020年11月30日21:57:57
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 更多内容敬请关注公众号：devstone



<center>
<img src="/res/img/blog/qrcode_for_devstone.jpg" width="50%" height="50%" />
</center>

---
