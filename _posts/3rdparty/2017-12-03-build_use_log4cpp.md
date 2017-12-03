---
layout: post
title: log4cpp编译及使用
categories: [开源库]
description: 编译log4cpp
keywords: Qt，log4cpp,日志
---

`Log4cpp` 是C++开源日志库，为 C++ 应用程序开发中提供了日志的追踪和调试功能。`Log4cpp` 基于 `LGPL` 开源协议，移植自 java 的日志项目 log4j, 并在 api 上保持了一致性。

## 环境

- windows 7 64b + Qt5.7 + MinGW530
- log4cpp: 1.1.3

## 下载

- 地址：[https://sourceforge.net/projects/log4cpp/](https://sourceforge.net/projects/log4cpp/)
- log4cpp主页:[http://log4cpp.sourceforge.net/](http://log4cpp.sourceforge.net/)

## 特点
- 可扩展，提供了多种记录方式：文件、命令行、内存、win事件日志...
- 可以动态控制日志级别，进行动态调整
- 多语言支持,C(log4c), C++(log4cpp/log4cplus), java(log4j), python(log4p))


## 编译

打开工程后，可以发现包含了多个工程编译项目，不过基本都是基于VS的项目，不是我想要的，需要手动转化成 Qt 项目，也就是编程 `pro` 形式。


拷贝源码目录下的 `include` 和 `Src` 文件夹到自己新建的工程中，主要使用这两部分，其余的先不管。

**编译 pro 文件**

```C++
QT       = core

TEMPLATE = lib
TARGET = log4cpp
INCLUDEPATH += $$PWD/.. \
               $$PWD/../../boost/ \
DEPENDPATH += $$PWD/..

DEFINES += LOG4CPP_HAVE_BOOST

SOURCES += \
	Appender.cpp \
	AppenderSkeleton.cpp \
	AppendersFactory.cpp \
	......

HEADERS += \

win32:{	LIBS += -L -lwsock32 -lws2_32	}

```
上述引入了 `boost 库路径，并添加了wsock32库！否则编译会出现错误。

编译成功后会生成对应的库文件(静态库只有629KB):

![](/res/img/blog/3rdparty/log4cpp_lib.png)


## 使用