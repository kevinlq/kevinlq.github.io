---
layout: post
title: Qt Creator 源码学习笔记01，初识QTC
categories: [Qt]
description: Qt Creator 源码学习笔记 01
keywords: Qt, 源码,Qt Creator
---

> 阅读本文大概需要 4 分钟


`Qt Creator` 是一款开源的轻量级 `IDE`，整个架构代码全部使用 `C++/Qt` 开发而成，非常适合用来学习`C++`和`Qt` 知识，这也是我们更加深入学习`Qt`最好的方式，学习了解官方人员是如何开发一款软件的

## 下载

一般我们下载`Qt`进行安装的时候会默认安装好`Qt Creator`，此时安装的是可执行程序，而我们今天要讲的是下载源码并且编译，那么首先要到官方仓库中下载某个版本的源码

下载方式有两种，下载zip源码包和`gigt clone 获取`

[直接下载，打开通用下载页面下载](https://download.qt.io/official_releases/qtcreator/])

由于目前更新速度比较快，导致版本很多，不建议直接下载最新版本，这里以4.6版本为例

![](https://gitee.com/devstone/imageBed/raw/master/images/202110312327818.png)

通过这种方式下载下来直接解压就能看到整个框架源码

![](https://gitee.com/devstone/imageBed/raw/master/images/202110312331077.png)

另一种通过`git`方式下载源码更推荐，下载下来后你相当于拥有了所有版本的源码了，想要学习那个版本直接切换就行了，非常方便，下载地址有两个

- 官方: https://code.qt.io/cgit/qt-creator/qt-creator.git
- GitHub 镜像: https://github.com/qt-creator/qt-creator.git

命令分别为

```
 git clone git://code.qt.io/qt-creator/qt-creator.git
 
 git clone https://github.com/qt-creator/qt-creator.git
```

![](https://gitee.com/devstone/imageBed/raw/master/images/202110312350289.png)

可能有时候遇到网速慢等原因，下载会持续一段时间，如果等了好久还是下载慢，那么建议直接选择`Download Zip`，一般非常快

下载完成后，进入该源码目录执行切换版本命令即可



## 编译

源码下载完成后就可以开始动手直接尝试编译了，先不用详细了解每个目录是干啥的，跑起来再说

开始编译前，确保你电脑已经安装好了`Qt`，并且最好是 `Qt 5.6.2`及以上的版本

为啥会是这个呢，打开配置文件就可以看到

```
#version check qt
!minQtVersion(5, 6, 2) {
    message("Cannot build $$IDE_DISPLAY_NAME with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.6.2.")
}
```

直接打开工程文件`qtcreator.pro`即可，等待其加载完成，打开plugin.pro配置文件，只保留`coreplugin`插件工程，其余的都注释掉，暂时用不到，我们开始只关心核心插件

加载完成后直接构建编译即可，如果不出意外等待大概 10 多分钟时间就可以编译完成，你可以看到如下界面


![](https://gitee.com/devstone/imageBed/raw/master/images/202111011131568.png)


- [Qt Creator 源码学习笔记01，初识QTC](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484658&idx=1&sn=55af25cd6e608fa9cb1452610928e71b&chksm=e85c0ac2df2b83d453d80f66fcdefca31e998a8ac0ba0226edbd80b437f99ed5184478805d8e&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记02，认识框架结构结构](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484676&idx=1&sn=c1bd8cbd78d8e098c22353f567cc3620&chksm=e85c0b34df2b822288a7baa016f4a64df47e5e119039836ff686cb2ad3a7e28a5808efc0c13a&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记03，大型项目如何管理工程](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484701&idx=1&sn=c69eb1edfb551c41690938423ca2ef7c&chksm=e85c0b2ddf2b823b1179f216e57ca91b9ce4068a0469e8ba062ab3596e9dc51ac05a1572da85&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记04，多插件实现原理分析](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484767&idx=1&sn=971c14682095a8a84a8161311400100f&chksm=e85c0b6fdf2b82799db871eafb6d0b2465c75d8020b1f87a1f5825f82edcad2c051b7d6e1c2c&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记 05，菜单栏是怎么实现插件化的？](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484822&idx=1&sn=eff40e19952a534fc0d97cab2417ce9e&chksm=e85c0ba6df2b82b017eb568adacd5b407f269cc2c2e7d2c36609fdda76b029bdcd70003fadba&token=193645825&lang=zh_CN#rd)



******

    作者:鹅卵石
    时间: 2021年10月01日
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

