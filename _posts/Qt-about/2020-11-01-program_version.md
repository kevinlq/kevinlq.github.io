---
layout: post
title: qmake奇淫技巧之字符串宏定义
categories: [Qt学习]
description: qmake定义字符串宏在源码中使用
keywords: Qt,qmake
---
> 阅读本文需要3分钟

自己亲手编译过 Qt Creator 源码的朋友都有这样的疑惑，为啥编译完成后最终生成的动态库、插件以及 exe 都会带数字 4 后缀，下面记录下如何出现以及如何解决

```plain
Aggregationd4.dll
```
一般我们在构建程序时一个简单的工程 pro 是这样的
```c++
TEMPLATE = app
TARGET = $$IDE_APP_TARGET
DESTDIR = $$IDE_BIN_PATH
QT = core
HEADER  += \
    ...
SOURCE  += \
    ...
```
这样在执行 qmake 构建后最终生成的文件不会包含版本信息，我们来看下这个库的信息

![Aggregationd.dll信息](https://uploader.shimo.im/f/Zzuz2Pw36sckpNav.png)

那么如何才能加入版本信息呢？

其实很简单，版本信息通常有两种方法可以实现

* 手动编写 .rc文件
* 自动生成.rc文件

手动编写 .rc 文件方法比较繁琐，直接找到某个模板修改即可，下面说说最简单的自动生成方法

在 pro 文件添加版本定义变量

在你需要添加版本的工程 pro文件中添加下面这行代码

```c++
VERSION = 4.4.1
```
然后重新构建，此时你会发现在工程临时文件夹目录多了一个文件

Aggregation_resource.rc

```c++
# if defined(UNDER_CE)
#  include <winbase.h>
# else
#  include <windows.h>
# endif
VS_VERSION_INFO VERSIONINFO
	FILEVERSION 4,4,1,0
	PRODUCTVERSION 4,4,1,0
	FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
	FILEFLAGS VS_FF_DEBUG
#else
	FILEFLAGS 0x0L
#endif
	FILEOS VOS__WINDOWS32
	FILETYPE VFT_DLL
	FILESUBTYPE 0x0L
	BEGIN
		BLOCK "StringFileInfo"
		BEGIN
			BLOCK "040904b0"
			BEGIN
				VALUE "CompanyName", "\0"
				VALUE "FileDescription", "\0"
				VALUE "FileVersion", "4.4.1.0\0"
				VALUE "LegalCopyright", "\0"
				VALUE "OriginalFilename", "Aggregation4.dll\0"
				VALUE "ProductName", "Aggregation\0"
				VALUE "ProductVersion", "4.4.1.0\0"
			END
		END
		BLOCK "VarFileInfo"
		BEGIN
			VALUE "Translation", 0x0409, 1200
		END
	END
/* End of Version info */
```
该文件描述了这个库的版本等详细信息，然后查看动态库属性，文件版本信息都已经写进去了

![Aggregationd.dll信息](https://uploader.shimo.im/f/MmLLAL7upB6z4hMI.png)

好了，下面回到开始的问题，在编译 Qt Creator 源码后，为啥生成的动态库会包含数字 4 后缀，其实主要原因就是上述说的这个原因，qtcreator.pri 文件中定义了版本号变量，所以生成的库名称后面会追加数字4


[Qt官方文档说明](https://doc.qt.io/qt-5/qmake-variable-reference.html, "qmake描述文档")

![](https://uploader.shimo.im/f/qryNCuEsT1Oyo8d4.png)

如何修改呢？其实知道的问题的原因后，修改起来也很简单，主要有两种方法

* 删除 pro文件中 VERSION 字段定义；
* 手动编写 .rc 文件，手动编写后 qmake 不会给你自动生成，然后可以自定义名称了；


******

    作者:鹅卵石
    时间: 2020年11月18日20:08:30
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
