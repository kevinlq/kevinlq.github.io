---
layout : post
title: OpenCV学习之路之开发环境搭建(w7+ Qt4+opencv2+mingw)
category : 环境搭建
wangyiyun: true
date : 2016-12-17
---

******

	作者:鹅卵石
    时间:2016年12月17日11:40:31
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

# OpenCV学习之路之开发环境搭建(w7+ Qt4+opencv2+mingw)

>因为项目需要，这段时间开始学习opencv了，不得不说道路还是挺漫长的，之前对这个图形库有所了解，不过都是概念型的，现在要真正应用起来，内心还是挺欣慰的，好了废话不多说了，学习某一门技术搭建环境是必不可少的一件事，opencv的搭建还算不难。


----------


>环境:windows7 64b;
>Qt 4.8.6
>mingw3.8.6
>cmake:3.7.0

### opencv下载
opencv目前最新的版本是3.1.0(2016年12月17日22:57:42)，不过同事建议我下载opencv2开始学习，一来是因为opencv2这方面资料较多，二来是因为opencv3有好多api变化了，听说不是很稳定相比opencv2，所以我果断下载了opencv2.4.11.
下载地址：[http://opencv.org/downloads.html](http://opencv.org/downloads.html)
进去后可以看到如下界面： 

![opencv源码下载](/res/img/blog/环境搭建/opencv_source.png)

这里根据需要下载window版本的即可，如果是其他OS,对应下载即可。
点击下载后可以看到进入到了如下页面：

![源码下载](/res/img/blog/环境搭建/opencv_source2.png)

下载完成后等待安装即可。安装过程其实是一个自解压过程，opencv其实是会自动解压到我们选择的路径。

----------
### Qt+OpenCV2.4.11编译
OpenCV安装完成后生成了2个主要的文件夹：
Build和source文件夹，build文件夹下是编译好的一些开发库(MSVC编译器)，因此如果使用mingw编译器的话就得自己编译了。
#### 编译需要的工具

 1. Qt Creator
 2. mingw编译器
 3. CMake
 4. OpenCV

其实主要是需要CMake编译工具，上面2个在安装Qt的过程中应该第一个被安装，安装完成后都加入到环境变量中。需要注意的是在安装CMake的过程中最好不要安装在有空格、中文路径下，可以直接安装在某个盘下，比如我的：

![cmake](/res/img/blog/环境搭建/Cmake.png)

**编译**
首先要配置CMake的一些参数。
最为重要的两条路径：

![cmake配置](/res/img/blog/环境搭建/Cmake2.png)

上面的1是OpenCV源码路径，2是编译后要生成的路径，build-mingw是自己手动创建的。
配置完成后点击configure，弹出下面选择框：

![cmake编译选择](/res/img/blog/环境搭建/Cmake3.png)

选择红色圈起来的两条，后面要选择自己的编译器了

![cmkae编译](/res/img/blog/环境搭建/Cmake4.png)

点击完成之后便开始进行编译

![开始编译](/res/img/blog/环境搭建/Cmake5.png)

如果没有错误，可以顺利看到上面的截图，在configure完成后，点击Generate，显示Generate done.

### 使用mingw编译opencV
打开cmd,进入到上面设置的目标路径（即编译路径），输入mingw32-make进行编译：


![mingw编译](/res/img/blog/环境搭建/mingw.png)

#### 编译出现的错误

![出现的错误](/res/img/blog/环境搭建/mingw_error.png)

#### 解决方案
上述问题原因是找不到一些变量，通过百度找到了类似的解决方案：

![解决方案](/res/img/blog/环境搭建/mingw_error_q.png)

![解决方案2](/res/img/blog/环境搭建/mingw_error_q2.png)

解决问题后继续编译，如果顺利的话最终编译完成了，接下来就是安装了。
### 安装opencv
输入 mingw32-make install 进行安装：

![opencv安装](/res/img/blog/环境搭建/opencv_install.png)

继续安装
![opencv安装](/res/img/blog/环境搭建/opencv_install2.png)

可以看到安装成功了，这里安装的过程很快，不像编译时需要很久的，至少我用了快1个小时了，可能是自己本不给力吧。

### 编写demo测试
编译完成后会生成相应的库文件和头文件，如下图所示：

![测试](/res/img/blog/环境搭建/opencv_test.png)

#### 拷贝include文件夹
进入到install文件夹下，拷贝include文件夹到一个自定义的文件中，比如这里拷贝到了D:\OpenCV\opencv-2.4.11\necessary_file，这个文件夹随意，只要没有空格和中文都行，这是为了后面开发中要包含该路径。

![测试](/res/img/blog/环境搭建/opencv_test2.png)


#### CMake文件复制
找到CMake目标路径下的bin文件夹(注意不是CMake工具安装路径下的bin，而是编译opencv时选择的目标文件夹下的bin目录)

![cmake复制](/res/img/blog/环境搭建/cmake_copy.png)

拷贝lib文件夹到necessary_file文件夹下

![cmake复制](/res/img/blog/环境搭建/cmake_copy2.png)

拷贝完成后necessary_file文件夹应该是如下的样子：

![cmake复制](/res/img/blog/环境搭建/cmake_copy3.png)

拷贝完成后可以将该完整路径添加到环境变量中，为了程序中访问方便：
	D:\OpenCV\opencv-2.4.11\necessary_file
    至此所有的开发环境都搭建好了，剩下的就是编写一个简单的工程测试配置是否成功。

**demo出现了错误**
错误:cannot find D:\OpenCV\opencv-2.4.11\necessary_file\lib: Permission denied

![demo](/res/img/blog/环境搭建/opencv_demo.png)

权限错误可能应该是路径不正确导致的，换另一种加载库的方式可以成功！

![demo](/res/img/blog/环境搭建/opencv_demo2.png)

正确的配置方式(.pro)文件配置
```
INCLUDEPATH +=D:/OpenCV/opencv-2.4.11/necessary_file/include/opencv
INCLUDEPATH +=D:/OpenCV/opencv-2.4.11/necessary_file/include/opencv2
INCLUDEPATH +=D:/OpenCV/opencv-2.4.11/necessary_file/include
LIBS    +=-L D:/OpenCV/opencv-2.4.11/necessary_file/lib/libopencv_*.a
```
上述配置库的路径只写了一条，可以包含所有的相关动态库，刚开始是将所有的库都添加了路径，结果找不到。

![](/res/img/devstone.png)
