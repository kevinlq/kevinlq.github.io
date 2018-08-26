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

##  windows 平台编译
### 版本说明
- DCMTK:  3.6.2
- MinGW: 5.30.32
- Qt: 5.7.0

DCMTK工程使用CMAKE进行管理，本次为了方便集成到项目中，直接使用MinGW进行编译，这个时候需要自己梳理清楚各个模块之间的依赖关系，重新编写pro文件进行编译配置。

直接使用CMAKE进行配置编译也是很顺利的.  
<center>
<img src="/res/img/blog/medical_image/dcmtk_build_cmake.png" width="50%" height="50%" />
</center>

**每个模块均需要单独编译，所以每个模块对应一个pro文件**

**需要配置好库的生成路径，以及针对不同平台进行编译时生成的库的信息，要不然不同平台下会出错哦**

```
后面针对不同平台给这些变量进行赋值即可
比如 `win32` 
win32:{
	CONFIG(MinGW, MinGW|MinGW32|MinGW64|MSVC|MSVC32|MSVC64):{
		DIR_PLATFORM = Win32
		DIR_COMPILER = MinGW
		FILE_LIB_PREFIX = lib
		FILE_LIB_EXT = .a
	}
	else:CONFIG(MinGW32, MinGW|MinGW32|MinGW64|MSVC|MSVC32|MSVC64):{
		DIR_PLATFORM = Win32
		DIR_COMPILER = MinGW
		FILE_LIB_PREFIX = lib
		FILE_LIB_EXT = .a
	}
	......

# 始终编译成静态库
CONFIG += staticlib
```

> 看到每个模块里面都包含了 `docs` 和 `data`、`test`等文件夹，这里为了节省空间，直接去掉了，编译的时候用不到。

从上面的依赖关系可以看出，处于最底层的是 `config` 模块，因此首先配置好这个模块,这个模块暂时不需要编译，只是在编译其他模块的时候需要配置到其 `include` 路径下，暂时先不管了，等编译其他模块的时候在详细看。

### 编译ofstd模块

![](/res/img/blog/medical_image/dcmtk_build_ofstd.png)

提示找不到 `arith.h`头文件，经google该文件是用普通camke编译生成的文件，幸好之前用cmake编译过，直接拿过来就是了.

![](/res/img/blog/medical_image/dcmtk_build_config.png)

### 编译oflog模块

引入到工程中，编写pro配置文件

找不到这个结构: `OFunique_ptr`

![](/res/img/blog/medical_image/dcmtk_build_oflog.png)

没有找到声明或者定义该结构的地方，不得不放弃了，对比了下 3.6.0 版本的文件，确实少了好多，看来还是不能追求新版本啊。

直接使用3.6.0版本进行编译，没有问题.(以后对DCMTK熟悉了再试试)

### 编译dcmdata模块

添加到工程中，配置 `dcmdata.pro` 文件

发现 dcmdata模块下面还有一个 `libi2d` ,应该也需要编译的，看来也是依赖 `dcmdata`。

`libi2d`.（libi2d是用于导入BMP文件的）

![](/res/img/blog/medical_image/dcmtk_build_dcmdata.png)

编译OK！

### 编译 dcmimgle 模块

dcmimgle 模块依赖之前编译的其他模块>dcmdata>oflog>ofstd>config

依次添加并引入其他模块即可.

![](/res/img/blog/medical_image/dcmtk_build_dcmimgle.png)

### 编译 dcmimage 模块

### 编译 dcmjpeg 模块(编解码)

dcmjpeg 模块依赖一些库文件:

- libijg8
- libijg12
- libijg16

先编译上述库在引入

### 编译 dcmjpls 模块

同样 也是依赖一个库:

- libcharls

### 编译 dcmnet 模块

### 编译 dcmsr 模块

### 编译 dcmsign 模块

### 编译 dcmqrdb 模块

### 编译 dcmsign 模块

### 编译 dcmqrdb 模块

### 编译 dcmtls 模块

### 编译 dcmpstat 模块


> 注：在DICOM传输语法中，用到了 DCMJP2K ，这个库需要单独下载:

![](/res/img/blog/medical_image/dcmtk_build_jp2k.png)

### 最终模块截图

![](/res/img/blog/medical_image/dcmtk_build_all.png)

##  android 平台编译

上述模块配置好以后，需要专门针对不同平台进行配置，也就是 `config`模块的配置了，因此需要专门针对不同平台编写配置文件，某些平台下的宏定义不一样，例如：

![](/res/img/blog/medical_image/dcmtk_build_platporm.png)

在编译 `dcmdata` 模块时出现了错误：

![](/res/img/blog/medical_image/dcmtk_build_android.png)

只好修改这个地方:

```
/* Define to 1 if you have the `getlogin' function. */
//#define HAVE_GETLOGIN 1
#undef HAVE_GETLOGIN

```
编译通过!

## linux 平台编译

## MAC 平台编译

---

## 测试

在测试工程中引入DCMTK相关库文件和头文件：

```C++
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}DCMTK
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}ofstd
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}oflog
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}dcmdata
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}dcmimgle
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}dcmimage
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}dcmjpeg
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}dcmjp2k
DEPENDPATH += $${DIR_DEPEND_DCMTK_DEST}dcmjpls
#静态库文件
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -ldcmimage$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -ldcmimgle$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -ldcmjpeg$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -ldcmdata$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -loflog$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -lofstd$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -llibijg8$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -llibijg12$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -llibijg16$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -ldcmjp2k$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -ldcmjpls$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DCMTK_DEST} -llibjasper$${FILE_POSTFIX}

```

### 出现的错误

#### 错误1

![](/res/img/blog/medical_image/dcmtk_test_error.png)

链接出现了问题.谷歌说需要引入 `wsock32`

```C++
win32:{
    CONFIG(MinGW, MinGW|MSVC):{
            LIBS += -L -lwsock32 -lws2_32
    }
    else:CONFIG(MSVC, MinGW|MSVC):{
            LIBS += -lwsock32 -lws2_32 -lAdvapi32
    }
}

linux:{	LIBS += -L$${DIR_DEPEND_DEST} -liconv -lcharset	}
```

#### 错误2
```C++
E: can't change to unencapsulated representation for pixel data
E: can't determine 'PhotometricInterpretation' of decompressed image
E: mandatory attribute 'PhotometricInterpretation' is missing or can't be determined
```
编解码问题!


## 使用

如何使用见这篇文章[使用](http://kevinlq.com/2017/12/05/DCMTK-learn/)

******

    作者:鹅卵石
    时间:2017年11月25日14:15:51
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="/res/img/myCode.png" width="80%" height="80%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---


