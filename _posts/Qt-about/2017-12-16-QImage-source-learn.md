---
layout: post
title: QImage源码分析之Save方法实现
categories: Qt学习
description: QImage保存图像分析
keywords: QImage,save
---

## 起因

最近使用到了QImage操作图像数据,对image的像素数据处理完后，进行save的时候出现了错误，自己电脑没有错误，程序跑的云主机上就会出现一些错误，最终通过 log 定位到是在save的时候出现了错误，一直失败。幸运的是最后问题最终解决了，今天刚好有时间抽空看看源码是如何实现的.

通过大概粗略的看了下源码，发现save方法依赖具体你传递的format，然后再进行具体处理.

比如 png格式的图片最终由 `QPngHandler`类进行处理，在处理过程中使用到了第三方库`libpng库`，这就很好解释我遇到的问题了，需要图片库支持，最好和自己开发时所使用的库匹配!

![](/res/img/blog/Qt-learn/QImage/format_lib.png)

>需要检测这些和QImage相关的库是否齐全!

---

## 简单分析

### QImage类声明

```C++
class Q_GUI_EXPORT QImage : public QPaintDevice
{
public:
    ......
    QImage();
    QImage(const QSize &size, Format format);
    QImage(int width, int height, Format format);
    ......
    bool save(const QString &fileName, const char *format = Q_NULLPTR, int quality = -1) const;
    bool save(QIODevice *device, const char *format = Q_NULLPTR, int quality = -1) const;
    ......
private:
    ......
    QImageData *d;
    ......
};
```
>可以看出Qt一贯的 `d` 指针又出现了.

### QImage实现

```C++
//构造一个空的image,d指针为0
QImage::QImage()
    : QPaintDevice()
{
    d = 0;
}
//根据宽度和高度以及格式构造image，初始化d指针数据
QImage::QImage(int width, int height, Format format)
    : QPaintDevice()
{
    d = QImageData::create(QSize(width, height), format, 0);
}
```

>`QImageData` 是私有数据,在它的构造函数中又用到了`QImageData`

```C++
 QScopedPointer<QImageData> d(new QImageData);
```
可以看出使用到了智能指针`QScopedPointer`.其他代码先不看了，基本都是保存了一些图像参数(width,height,depth,nbytes....)

**save方法实现**
```C++
bool QImage::save(const QString &fileName, const char *format, int quality) const
{
    if (isNull())
        return false;
    QImageWriter writer(fileName, format);
    return d->doImageIO(this, &writer, quality);
}

bool QImage::save(QIODevice* device, const char* format, int quality) const
{
    if (isNull())
        return false;                                // nothing to save
    QImageWriter writer(device, format);
    return d->doImageIO(this, &writer, quality);
}
```
save 有两个方法，关键是第一个参数类型不同，但是都是属于`QIODevice`，也就说是IO类型的。

均是通过构造`QImageWriter`来实现.
### save流程

以下是大概的一个流程

![](/res/img/blog/Qt-learn/QImage/QImage-save.png)

越来越发现，阅读 Qt 的源码是一种享受.可能要沉迷于Qt源码了……


>本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。
但是，希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。
