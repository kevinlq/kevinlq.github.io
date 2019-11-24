---
layout: post
title: VTK Build
categories: [3rdparty]
description: VTK Build
keywords: Qt,VTK
---

## 简介

## 准备环境

- CMake-3.6
- VTK-8.1
- MingW53-32
- Qt 5.11

## 配置

- BUILD_EXAMPLES  是否编译VTK例子(默认关闭)
- BUILD_TESTING 测试程序(默认打开)，先关闭
- BUILD_SHARED_LIBS 编译动态库(默认关闭)，可以根据需要选择是编译动态库还是静态库
- VTK_DATA_ROOT VTK数据路径(编译VTK例子时需要用到)
- CMAKE_INSTALL_PREFIX 安装路径,最好修改成自己希望的安装路径(避免因为权限问题安装失败)
- VTK_USE_QT 是否使用Qt,勾选
- VTK_WRAP_JAVA,VTK_WRAP_PYTHON,VTK_WRAP_TCL 暂时不需要，因为这里使用C++开发，如果需要其他解释型语言开发可以勾选
- BUILD_DOCUMENTATION 编译帮助文档(默认关闭),这里不需要修改
- VTK_USE_GUISUPPORT 是否支持GUI，如果在Windows下开发使用MFC，则需要打开该选项


![cmake 配置](https://gitee.com/devstone/raw/master/blog/3rdparty/VTK/build_cmake_setting.png)

## 编译

- configure

在这里会提示`tiff`库找不到，报错，需要配置`tiff`库以及头文件，错误解决办法见文章后面介绍;

- generate

- 打开Mingw32命令窗口开始编译

```C++
mingw32-make -j4
```

![start build ](https://gitee.com/devstone/raw/master/blog/3rdparty/VTK/build_ok.png)

等待大概1个小时左右，喝杯咖啡看会电影差不多就结束了

编译成功后开始安装,编译工程直接引入

```
mingw32-make install
```

![build](https://gitee.com/devstone/raw/master/blog/3rdparty/VTK/install_ok.png)


## 测试

```
#include "vtkRenderer.h"

#include"vtkRenderWindow.h"

#include"vtkSmartPointer.h"

int main()

{

 vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();

   renWin->Render();

   std::cin.get();

   return 0;

}
```

## 问题

使用最新版本 `VTK`在编译过程中，会遇到一些问题，比如:

- tiff 库问题

![cmake 配置](https://gitee.com/devstone/raw/master/blog/3rdparty/VTK/build_error_tiff.png.png)


编译时提示 `tiff`库中某个文件，最终出现问题提示是mingw问题了，先没有用`vtk`自带的`tiff`库，从网上下载完自己编译

开始编译

![cmake 配置](https://gitee.com/devstone/raw/master/blog/3rdparty/tiff/tiff_build_start.png)

编译成功

![cmake 配置](https://gitee.com/devstone/raw/master/blog/3rdparty/tiff/tiff_build_ok.png)

安装成功:

![cmake 配置](https://gitee.com/devstone/raw/master/blog/3rdparty/tiff/tiff_install_ok.png)

## 帮助

![](https://gitter.im/notepad-plus-plus/notepad-plus-plus)

- 找不到PDF某个功能

具体原因不详，这里也暂时不用到，将相关文件屏蔽掉

![](https://gitee.com/devstone/raw/master/blog/3rdparty/VTK/build_error_pdf.png)

- 找不到库文件

```
undefined reference to `_imp__getstockobject@4'
```

******

    作者:鹅卵石
    时间: 2019年11月23日21:49:38
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 关注

<center>
<img src="https://gitee.com/devstone/imageBed/raw/master/code/qrcode_for_devstone.png" width="50%" height="50%" />
</center>

---
