---
layout : post
title: unused parameter warnings in C code
category : C学习
wangyiyun: true
date : 2017-04-04
---

******

    作者:鹅卵石
    时间:2017年4月4日13:47:11
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->


# unused parameter warnings in C code

今天在整理相关控件过程中出现了一些编译警告，有了警告当然不能忽略，发现都是一些函数参数声明了但是没有使用，这个事好办，直接加个void就行

## 问题描述

![问题描述](/res/img/blog/C学习/001.png)

## 解决方案

添加宏定义
```
#define UNUSED(x) (void)(x)
```
![解决方案](/res/img/blog/C学习/002.png)

Qt中也有类似的宏定义，Q_UNUSED()
点击进去源码看了下，具体实现是这样的：

qglobal.h 620h
```
/*
   Avoid "unused parameter" warnings
*/

#if defined(Q_CC_RVCT)
template <typename T>
inline void qUnused(T &x) { (void)x; }
#  define Q_UNUSED(x) qUnused(x);
#else
#  define Q_UNUSED(x) (void)x;
#endif
```
可以看出Qt处理的很奇妙，代码很简洁，很值得去学习学习

![效果](/res/img/blog/C学习/003.png)

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)