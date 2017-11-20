---
layout : post
title: STM32启动文件选择问题
category : STM32学习
wangyiyun: true
date : 2015-09-08
---

******

    作者:鹅卵石
    时间:2015年09月08日23:04:16
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

# STM32启动文件选择问题

关于stm32启动文件的选择

- startup_stm32f10x_ld_vl.s: for STM32 Low density Value line devices
- startup_stm32f10x_ld.s: for STM32 Low density devices
- startup_stm32f10x_md_vl.s: for STM32 Medium density Value line devices
- startup_stm32f10x_md.s: for STM32 Medium density devices
- startup_stm32f10x_hd.s: for STM32 High density devices
- startup_stm32f10x_xl.s: for STM32 XL density devices
- startup_stm32f10x_cl.s: for STM32 Connectivity line devices


----------


* cl：互联型产品，stm32f105/107系列
* vl：超值型产品，stm32f100系列
* xl：超高密度产品，stm32f101/103系列
* ld：低密度产品，FLASH小于64K
* md：中等密度产品，FLASH=64 or 128
* hd：高密度产品，FLASH大于128
