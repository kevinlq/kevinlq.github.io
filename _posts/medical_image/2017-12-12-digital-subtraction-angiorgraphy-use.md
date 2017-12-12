---
layout: post
title: DSA 血管造影知识
categories: 医学影像
description: DSA 血管造影基础概念知识
keywords: DSA
---

数字减影血管造影(Digital Subtraction Angiorgraphy，简称 `DSA`),是利用计算机处理数字化的影像信息，以消除骨骼和软组织影的减影技术。

>在给血管进行造影时，因为血管与骨骼和软组织影重叠，显得血管影不清楚，以前使用光学减影技术可以消除这些影像，随着计算机技术的发展，使用计算机进行处理更加准确方便。

## 检测部位

DSA不但能清楚地显示颈内动脉、椎基底动脉、颅内大血管及大脑半球的血管图像，还可测定动脉的血流量，所以，目前已被应用于脑血管病检查，特别是对于动脉瘤、动静脉畸形等定性定位诊断，更是最佳的诊断手段。不但能提供病变的确切部位，而且对病变的范围及严重程度，也能清楚地了解，为手术治疗提供可靠的客观依据。另外，对于缺血性脑血管病，也有较高的诊断价值。DSA可清楚地显示动脉管腔狭窄、闭塞、侧支循环建立情况等，对于脑出血、蛛网膜下腔出血，可进一步查明导致出血的病因，如动脉瘤、血管畸形、硬脑膜动静脉瘘等。

![](/res/img/blog/medical_image/DSA.jpg)

## 原理

先使人体某部分在影像增强器影屏上成像，用高分辨率摄像管对影屏上的图像进行`行序列扫描`，把影屏上的图像分成一定数据量的小方块，即像素。再经过模拟/数字转换器转成数字，并按照序列排成字矩阵，这样，图像就被像素花和数字化了.

![](/res/img/blog/medical_image/dsa_trans.png)

---

在注入造影剂前，首先摄取一些列连续`掩膜图像`，然后选择差值图像直方图的能量作为相似测度，将造影图像与`掩膜`序列图像进行全局配准，找到其中匹配的掩模图，再用块匹配的方法对该造影图像和普配的掩膜图像进行局部配准减影。

- 图 a 为造影图
- 图 b 为与图 a 匹配的掩模图
- 图 c 为图 a 与 图 b 直接减影图像
- 图 d 为经过局部匹配后的减影图像

![](/res/img/blog/medical_image/dsa_result.png)

可以看出经过局部匹配后的减影图像质量较直接减影的图像质量好.

## 相关算法

```C++
template <class T>
bool Substraction(T *pMask, T *pContrast, T *pResult, int nLength, int nWidth, 
		T nMinLimit, T nMaxLimit, int nSamplesPerPixel)
{
	if( pMask == NULL || pContrast == NULL || pResult == NULL || nLength <= 0 || nWidth <= 0 )
    {
        return false;
    }

    long lRange = nMaxLimit - nMinLimit;
    long lSize = nLength * nWidth * nSamplesPerPixel;

    T *pMaskTemp		= pMask;
    T *pContrastTemp	= pContrast;
    T *pResultTemp		= pResult;

    float fTemp = 0.0f;

    for( long i = 0; i < lSize; i++ )
    {

        if( *pContrastTemp < ( lRange / 40 ) && *pContrastTemp < 6 )
        {
            *pResultTemp = 0;
        }
        else
        {
            fTemp = exp( -m_fRescaleSlope*( float(*pContrastTemp - *pMaskTemp) - m_fRescaleCenter ) );

            *pResultTemp = nMinLimit + T(float( lRange ) /( 1.0f + fTemp ));
        }

        pMaskTemp++;
        pContrastTemp++;
        pResultTemp++;
    }

    return true;
}

```


## 参考文章

[周正东:心脏冠动脉数字造影图像增强研究](http://www.docin.com/p-503963245.html)
