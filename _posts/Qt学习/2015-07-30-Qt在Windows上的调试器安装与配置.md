---
layout : life
title: Qt在Windows上的调试器安装与配置
category : Qt学习
duoshuo: true
date : 2015-07-30
---

******

  如果安装Qt时使用的是Visual Studio的预编译版，那么很有可能就会缺少调试器（Debugger），而使用MSVC的Qt对应的原生调试器是CDB（对应MinGW的Qt使用GDB调试器）。本文介绍如何下载安装CDB，并在Qt Creator配置之。

示例环境：Windows 7 64bit + Visual Studio 2013 + Qt 5.5.0

<!-- more -->

## 选择合适的Windows Driver Kit（WDK）

CDB已经不再作为独立软件包，而是WDK中的一部分。虽然WDK是对应不同的Windows版本的，但是因为我们只需要CDB，而不开发驱动程序，基本上不用理会操作系统的要求，只需关注对应的Visual Studio版本即可。

> * WDK 7.1对应的是VS2010
> * WDK 8.0对应VS2012
> * WDK 8.1对应VS2013

WDK 8.0、WDK 8.1支持Windows 7、Windows Server 2008 R2、Windows8、Windows 8.1、Windows Server 2012，本文选用的就是WDK 8.1

从微软下载[WDK](http://msdn.microsoft.com/en-us/windows/hardware/hh852365)

得到的是一个不到1MB的在线安装程序，安装时，只需要选中Debugging Tools for Windows即可，其它都可以不安装，如下图所示:

![示例](/res/img/blog/Qt学习/daodaoliang.jpg)

安装完成后Close关闭安装程序。

## 配置Qt Creator

重新启动Qt Creator，在设置里面，Build & Run的Kits选项卡，可以看到Kits里面之前的黄色感叹号三角形没有了，而且Debugger也被自动识别到了。

![示例2](/res/img/blog/Qt学习/qtcreator-cdb-8-300x173.jpg)

在Debugger面板里，CDB Path选项卡，点击Insert后边的小三角形选择Symbol Server，弹出的窗口填写存放Symbol的缓存文件夹。确认后如下图

![示例2](/res/img/blog/Qt学习/cdb-symbolpath.jpg)

不过第一次debug的时候，可能还是会提示Symbol Server的设置，一般直接确认就可以了。