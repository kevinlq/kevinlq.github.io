---
layout: post
title: qmake奇淫技巧之字符串宏定义
categories: [Qt学习]
description: qmake定义字符串宏在源码中使用
keywords: Qt,qmake
---
> 阅读本文大概需要3.3分钟


我们平时在软件开发过程中需要定义一些宏，以便在代码中调用，这样每次不需要修改代码，只需要修改外部编译命令就可以得到想要的参数，非常方便

比如我们想在软件介绍中显示软件版本，让别人知道当前使用的软件是那个版本，一般会怎么做呢？

- 直接在代码中写死，版本号是多少
- 从外部配置读取显示
- 编译的时候通过编译条件读取，然后代码中读取对应变量值

今天看看平时使用最多的 `Qt Creator` 是怎么处理的

随便打开 `Qt Creator`的关于软件可以看到

![](https://gitee.com/devstone/imageBed/raw/master/images/20201031212921.png)

通过阅读源码就可以很清楚的知道，`Qt Creator`是通过在 `Pro` 配置文件中定义宏，然后在源代码中直接读取显示的

下面来简单说说如何实现，主要包含下面几个过程

1. 在 `pro`文件中定义字符串宏
2. 宏定义字符串
3. 在源码中直接调用


 感兴趣的朋友可以下载 `Qt Creator` 源码详细查看，墙裂学习 `C++` 的朋友去读读源码，真的可以让你学到很多意想不到的知识
 
 ## pro 定义字符串宏
 
 我们打开源码 `qtcreator.pri`文件，就可以看到关于版本号的定义
 
 ```
QTCREATOR_VERSION = 4.4.1
 ```
上面 `QTCREATOR_VERSION` 变量定义了一个版本号，在其他配置文件中也可以直接读取该变量

但是，仅仅定义一个变量是不够的，在源码中是无法读取到的，直接编译会报错

## 宏定义字符串

宏定义就是这样一个简单语句

```
DEFINES += QTCREATOR_VERSION=\"\\\"$$QTCREATOR_VERSION\\\"\"
```

在搞清楚上述语法之前，必须要知道反斜杠转义的知识 [反斜杠转义](https://support.google.com/analytics/answer/1034332?hl=zh-Hans, "反斜杠转义")

```
\\（对反斜杠转义）
\[（对中括号转义）
\{（对花括号转义）
\.（对点转义）
```

**说明**

![](https://gitee.com/devstone/imageBed/raw/master/images/20201031220650.png)

- 第一个反斜杠+引号(首尾成对)，表示要对引号转义，告诉 `qmake`里面是一个字符串；
- 第二个反斜杠对(2个连续反斜杠)，也是转义，表示对反斜杠本身进行转义，最终在 `makefile`中变成一个反斜杠
- 第三个反斜杠+引号是对引号进行转义，表示里面最终是一个字符串

因此，上述语句最终会生成一个字符串，后面我们直接可以使用该字符串宏

## 在源码中直接调用

```
#include "Widget.h"
#include <QApplication>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << APP_NAME << QTCREATOR_VERSION;

    return a.exec();
}


debug out

QtCreator 4.4.1

```

很轻松就可以定义字符串宏在代码中使用，这个技巧还是很方便的

### 总结

在定义字符串宏的时候一定要注意，你要定义的字符串是否包含空格，如果没有空格，还可以简单这样写

```
DEFINES += APP_NAME=\\\"QtCreator\\\"
```

这是为什呢？我们通过 `makefile` 文件可以发现，如果你的字符串包含空格，那么处理后会是这个样子

```
-DAPP_NAME=\"QtCreator -D\" 
```
`qmake`在处理时后面多加了一个 `-D`,进而出现错误

```
<command-line>:0:10: warning: missing terminating " character
```
正确的 `makefile`文件处理过程

```C++
DEFINES = -DUNICODE -DQTCREATOR_VERSION="\"4.4.1\"" -DAPP_NAME=\"QtCreator\"
......
```

后续关于 `Qt Creator`源码分析会有一个系列进行总结，尽请关注

`Qt Creator` [代码](http://download.qt.io/archive/qtcreator/4.4/4.4.0/qt-creator-opensource-src-4.4.0.tar.xz.mirrorlist, "源码下载地址")是开源的，感兴趣的可以直接去下载阅读


******

    作者:鹅卵石
    时间: 2020年11月1日14:56:48
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
