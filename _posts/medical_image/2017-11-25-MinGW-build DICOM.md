---
layout: post
title: windows下mingw编译DCMTK库
categories: [医学影像, MinGW]
description: window下将DCMTK库集成到自己项目中进行编译
keywords: MinGW, DCMTK
---

## 概述

`DCMKT`是由德国 `offis` 公司开发的开源库，实现了 `DICOM` 协议的一个平台，使得我们可以在它的基础上轻松完成自己的主要工作，而不必在意细节。目前支持 `Linux`、`windows`、`MACOS`、`SUN`等操作系统，可以说是很方便开发跨平台应用。

## 使用

### 下载

- 公司主页 [http://dicom.offis.de/](http://dicom.offis.de/)
- 下载地址:[http://dicom.offis.de/dcmtk.php.en](http://dicom.offis.de/dcmtk.php.en)
- 常见问题论坛:[http://forum.dcmtk.org/](http://forum.dcmtk.org/)

下载后直接解压，会看到如下目录结构：

![](/res/img/blog/medical_image/dcmtk_file.png)

### 功能模块学习

- **config:** 关于DCMTK的编译设置
- **ofstd:** 一些通用库
- **oflog:** 日志系统库
- **dcmdata:** 包含了最基本的`DcmItem`、`DcmElement`、`DcmTag`等标志.
- **dcmimgle:** 图像(灰度)处理模块
- **dcmimage:** 彩色图像处理部分
- **dcmnet:** DICOM网络协议相关
- **dcmqrdb:** 测试PACS
- **dcmsr:** 结构化报告
- **dcmwlm:** 工作列表
- **dcmjpeg:** JPEG编解码器相关
- **dcmjpls:** JPEG-LS编解码器相关
- **dcmpstat:** 表示状态
- **dcmsign:** 签名
- **dcmtls:** 网络安全相关
- **docs:** 通用文档
- **doxygen:** 文档的doxygen接口

### 模块间的依赖关系

![](/res/img/blog/medical_image/module_dependencies_2011.png)

## 编译
DCMTK工程使用CMAKE进行管理，本次为了方便集成到项目中，直接使用MinGW进行编译，这个时候需要自己梳理清楚各个模块之间的依赖关系，重新编写pro文件进行编译配置。

**每个模块均需要单独编译，所以每个模块对应一个pro文件**

> 看到每个模块里面都包含了 `docs` 和 `data`、`test`等文件夹，这里为了节省空间，直接去掉了，编译的时候用不到。

从上面的依赖关系可以看出，处于最底层的是 `config` 模块，因此首先配置好这个模块,这个模块暂时不需要编译，只是在编译其他模块的时候需要配置到其 `include` 路径下，暂时先不管了，等编译其他模块的时候在详细看。
