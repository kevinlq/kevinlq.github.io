---
layout: post
title: 使用MinGW静态编译Opencv模块
categories: [OpenCV学习, 环境搭建]
description: some word here
keywords: OpenCV学习, opencv编译
---

## 环境说明
>OS:windows7 64b  
Qt 5.7.0  
MinGW:53032  
CMake:3.10.0-rc3-w64  

静态编译Opencv模块

这几天公司有个功能，需要用到一些图像处理部分算法，所以直接移植了opencv中imgproc模块和core模块。

以前编译opencv很容易，直接准备好环境，先用CMake生成Makefile文件，然后使用MinGW编译器进行编译，最后安装，完事。

**之前编译过程的文章**

[整体编译方法 &arr](http://kevinlq.com/2016/12/17/OpenCV%E5%AD%A6%E4%B9%A0%E4%B9%8B%E8%B7%AF%E4%B9%8B%E4%B8%80%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA(W7+Qt4+opencv2+mingw)/)

现在是要静态编译，而且要把源码集成到项目中来，所以需要自己编写pro工程来配置，想起来很简单，实际编译起来，遇到了很多问题,因为opencv模块中依赖了其他第三方库，所以编译过程还是挺艰难的.

## 编译前准备

准备好环境，opencv源码,CMake,Qt安装好.

## 编译

拷贝`core`和`imgproc`模块源码到自己工程下，大致结构如下所示(以下是自己测试工程结构)：

![](/res/img/blog/OpenCV学习/core-imgproc.png)

关键部分就三个，core模块、imgproc模块以及测试子工程.

关于这部分的源码我已经上传到个人github仓库了。

移植过程中国遇到了很多的问题，很多的解决方案都放在了github上。这里就不介绍了。

[连接地址](https://github.com/kevinlq/Opencv-module-build)

---

如果您对本文有任何问题，可以在下方留言，或者Email我 .微信公众号：devstone，扫描二维码直接关注
<center>
<img src="/res/img/blog/qrcode_for_devstone.jpg" width="25%" height="25%" />
</center>