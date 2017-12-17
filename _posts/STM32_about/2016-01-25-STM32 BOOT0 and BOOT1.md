---
layout : post
title: STM32之BOOT0和BOOT1
category : STM32学习
wangyiyun: true
date : 2016-01-25
---

******

    作者:鹅卵石
    时间:2016年01月25日10:04:16
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

# STM32之BOOT0和BOOT1

|BOOT0   |BOOT1     |启动模式     |说明     |
| ---  | --- | --- | --- |
|0     |X     | 用户闪存寄存器    |用户闪存存储器，也就是FLASH     |
|1     |0     |系统存储器     |系统存储器启动串口下载     |
|1     |1    |SRAM     |SRAM启动，用于在SRAM中调试代码     |


一般情况下，如果我们想利用串口下载代码，则必须配置BOOT0为1，BOOT1为0；而如果想让SRM32一按复位键就开始跑我们下载的代码，则需要配置BOOT0为0，BOOT1随便设置(一般为了统一设置为0)