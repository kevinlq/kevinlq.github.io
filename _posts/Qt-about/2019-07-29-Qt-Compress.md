---
layout: post
title: Qt 图像压缩以及Base64之间编解码
categories: [Qt学习]
description: 整理下 Qt 自带的图像压缩以及 base64编解码操作方法
keywords: Qt,base64,QImage
---


> 整理下 Qt 自带的图像压缩以及 base64编解码操作方法

## Qt 自带的压缩方法

Qt 自带了一个压缩/解压缩方法，在一般需求下=可以直接使用.

- 压缩

``` C++
QByteArray qCompress(const uchar *data, int nbytes, int compressionLevel = -1)

QByteArray qCompress(const QByteArray &data, int compressionLevel = -1)

其中 `compressionLevel` 代表压缩等级[0,9]之间，默认值为-1，使用zlib默认压缩，
数值越大，压缩后的数据越小，但是压缩速度较慢.

```

- 解压

```C++
QByteArray qUncompress(const uchar *data, int nbytes)
QByteArray qUncompress(const QByteArray &data)

```

使用 Qt 自带的方法进行压缩和解压时，必须确保要解压的数据是使用 `qCompress` 压缩后的，否则解压会失败，
原因是因为 Qt压缩时会在数据前面写入 4 个字节的数据字节数,如果使用的是外部压缩工具，解压时需要进行自动填充对应的字节数。


## Base64编解码

### 编码

> data:image/jpeg;base64, 前缀

- 简单文本

```
QByteArray array  = "KGgoAAAANSUhEUgAAAgAAAAIACAYAAAD0eN...";
QByteArray baseArray = "data:," + array.toBase64();

```

- 图像

```
QString strFileName = "D:/Project/Test/markImage.png";
QImage image(strFileName);

QByteArray array;
QBuffer buffer(&array);
image.save (&buffer, "png",100);
buffer.close ();

/*! 这里对image 先进行压缩*/
QByteArray baseArray = qCompress (array,9);

/*! 压缩后的数据转base64 */
QByteArray comImageArray = "data:image/jpeg;base64," + baseArray.toBase64 ();
comImageArray = qCompress (comImageArray, 9);

```

实际验证，这里进行了两次压缩，压缩效果还行，压缩前image保存成base64后大小为 1MB，对图像压缩后保存变为 6KB,再次压缩后保存变为 3KB。

注意 `"data:image/jpeg;base64," `字符串，必须有.

```
data:,文本数据

data:text/plain,文本数据

data:text/html,HTML代码

data:text/html;base64,base64编码的HTML代码

data:text/css,CSS代码

data:text/css;base64,base64编码的CSS代码

data:text/javascript,Javascript代码

data:text/javascript;base64,base64编码的Javascript代码

data:image/gif;base64,base64编码的gif图片数据

data:image/png;base64,base64编码的png图片数据

data:image/jpeg;base64,base64编码的jpeg图片数据

data:image/x-icon;base64,base64编码的icon图片数据
```

### 解码

解码的过程和编码对应，在编码的时候进行了几次这里也需要对应解码几次。

```
QImage tmpImage;
comImageArray = qUncompress (comImageArray);
// 如果数据中包含 [data:image/jpeg;base64,]，需要移除后才能加载紧 `QImage`
if (comImageArray.startsWith ("data:image/png;base64,"))
{
    comImageArray = comImageArray.replace ("data:image/png;base64,", "");
}

QByteArray fromBase64 = QByteArray::fromBase64 (comImageArray);

fromBase64 = qUncompress (fromBase64);

tmpImage.loadFromData (fromBase64);
bool bResult = tmpImage.save ("D:/Project/Test/title_base64_image.png", "png");
qWarning()<<bResult;

```

<center>
<img src="/res/img/blog/Qt-learn/qimage_base64.png" width="100%" height="100%" />
</center>



******

    作者:鹅卵石
    时间: 2019年7月29日21:48:07
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="/res/img/myCode.png" width="50%" height="50%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---
