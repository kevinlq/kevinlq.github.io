---
layout: post
title: 图像采样算法分析
categories: [算法学习]
description: 常见图像采样算法分析
keywords: 算法,采样
---

今天用到了图像抽样相关算法功能，特意学习了下相关处理的基本知识，在这里总结下，作为一份笔记吧。

## 采样或者抽样

我们修改图片的size(大小)过程就是一个图像重新采样的过程。放大图像称为上采样/插值(upsampling)，相反缩小图像称为下采样(downsampling)

## 相关算法

### 最近邻接插值算法

### 两次线性插值算法

### 两次立方插值算法 

###

## 具体实现

```C++

pDS				DcmDataset数据集
fScale			缩放比例
nMinSize		最小长宽尺寸
nMaxSize		最大长宽尺寸
nInterpolate	插值 0, 1, 2, 3, 4
nFrame			图像的帧数

static DicomImage *CreateScaledDicomImageScale( void* pImage, bool bDataSet,
												float fScale, int nMinSize = 0, 
												int nMaxSize = 0,int nInterpolate = 0,
												int nFrame = 0 );

```

## 参考文章
[关于图像重采样差值算法](https://www.cnblogs.com/wjgaas/p/3597095.html)
[]()

******

    作者:鹅卵石
    时间:2018年1月4日22:41:29
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**
