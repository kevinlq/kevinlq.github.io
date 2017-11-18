---
layout : post
title: ARM机器视觉库
category : 物联网
wangyiyun: true
date : 2017-04-02
---

******

    作者:鹅卵石
    时间:2017年4月2日22:41:18
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

# ARM机器视觉库


昨天在微信公众号中看到了一条消息，顿时比较兴奋，是什么呢？就是有了ARM机器视觉库了，好开心。这为搞嵌入式的人来说是无比幸福的，一直期待有这么一个库，现在终于来了。

## 什么是ARM计算机视觉库
ARM计算机库是为了ARM CPU和Mail GPU体系结构优化的一系列低级软件，面向各种不同的使用情形，包括：图像处理，计算机视觉和机器学习。他是免费的，但需要获得MIT开源许可证才能使用

## ARM计算库包含的内容

- 基本运行、数学和二元运算符函数
- 颜色处理(转换、通道提取等)
- 卷积过滤器(Sobel、Gaussian等)
- Canny边缘、Harris角点、光流等
- 金字塔(如Laplacians)
- HOG(方向梯度直方图)
- SVM(支持向量机)
- H/SGEMM(半精度和单精度一般矩阵乘法)
- 卷积神经网络构造块(激活、卷积、全连接、局部连接、归一化、池化、Softmax)

## ARM视觉库优势

- 可移植
CPU功能是使用NEON内部函数实现的开发人员可以针对其目标体系结构重新编译这些内部函数，也就是说可以将代码编译成32位和64位

该库GPU版本使用OpenGl标准API编写的内核程序组成，同样可以跨越多种处理器和体系结构移植

- 一站式解决方案

该库与操作系统无关，目前已经部署在了基于ARM的新式linux和android片上系统平台上

## NEON解释

NEON是通过SIMD(单指令多数据)引擎

通过NEON可以加速多媒体和信号的处理算法

NEON可以处理挡圈和未来的多媒体格式，从而改善用户体验

[参考资料http://blog.csdn.net/dongjun7357/article/details/6278502](http://blog.csdn.net/dongjun7357/article/details/6278502)

## 主要的视觉库

### 1. HPC

>HPC通过提高仿真吞吐量为客户增添价值。利用HPC资源，工程团队不仅能分析单个设计理念，更能分析多种设计方案。通过同时仿真多个设计理念，研发团队能够在进行物理测试之前的设计流程中，提前发现显著的工程改进，而且效果比单独采用原型设计更为有效

### 2. Ne10

>An open optimized software library project for the ARM® Architecture

[https://github.com/kevinlq/Ne10][1]

![](/res/img/blog/物联网/ne10.png)

这个是一个开源的C库，包含了一些列针对ARM大力优化过的常见处理密集型函数
[下载链接](https://github.com/kevinlq/Ne10)

### 3. libyuv

>是一个包含YUV缩放和转换的开源项目

### 4. skia

>是一个开源二维图形库，用做Google Chrome和Chrome OS、Android、Mozilla Firefox、Firefox OS以及其他很多产品的图形引擎


  [1]: https://github.com/kevinlq/Ne10

  
  动手写写以前的基础代码，还是有很多的好处的。发现某些细节还是遗忘了