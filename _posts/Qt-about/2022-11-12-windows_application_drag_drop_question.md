---
layout: post
title: Qt 编写的应用程序无法使用拖拽问题
categories: [Qt]
description: 当应用程序以管理员权限运行时无法使用拖拽功能交互
keywords: Qt, C++, Drag, drop
---


> 阅读本文大概需要 2 分钟

## 问题背景

接到产品经理的需求：给我们的程序添加一个打开导入文件的方式，允许用户拖动外部文件直接打开该文件，想了下也不难，没有几行代码，随口答应了下来

实际验证下来，发现有很大的坑

正常情况下，肯定使用没有问题，打开程序后随意拖动任何文件都可以感应到，但是当你的程序被用户以「管理员」方式运行时，此时拖拽功能就歇菜了

无论你怎么操作，拖拽文件事件始终检测不到……

## 原因

发现问题后，随即搜索了一圈，发现这个是 `Windows` 做了限制。有一个叫做 `UAC` 的概念先来解释下

> 在很早的 `XP` 时代还没有这个概念，从 `vista` 开始引入了一种安全机制(UAC User Account Control), 不同管理权限的程序是无法直接交互的，为了防止潜在的特权提升攻击，需要阻止某些交互功能。

出于安全考虑，`Windows` 禁止用户程序和管理员程序之间交互，我们的拖拽是消息，触发者是资源管理器(explorer.exe)默认开机后运行是非管理员权限，这样拖拽就有问题

正常权限程序向具有管理员权限的程序发送消息可能会导致安全问题，被禁用了。

## 方案

目前来看没有很好的方案来解决，只能在非管理员权限下使用程序，或者强制使 `explorer.exe` 以管理员权限运行，如果你的产品是面向 `C` 端的产品，那么这个方案肯定不行

好了，到此为止，可以有底气怼产品了，打扰了，无法实现……


## 参考文档

- [stackoverflow](https://stackoverflow.com/questions/37828617/drag-and-drop-not-working-if-application-lies-in-specific-directory)
- [Qt 论坛](https://forum.qt.io/topic/131155/qt-applications-cannot-drag-and-drop-files-when-running-under-windows-with-administrator-rights)
- [Microsoft](https://social.technet.microsoft.com/Forums/en-US/cba0e9b1-25f8-40e5-a888-1435d604f68d/quotrun-as-administratorquot-prevents-drag-and-drop-working?forum=w7itprosecurity)
- [Microsoft about drag and drop](https://learn.microsoft.com/en-gb/archive/blogs/patricka/q-why-doesnt-drag-and-drop-work-when-my-application-is-running-elevated-a-mandatory-integrity-control-and-uipi)
- [UAC](https://learn.microsoft.com/en-us/windows/security/identity-protection/user-account-control/how-user-account-control-works)


**推荐阅读**

******

    作者:鹅卵石
    时间: 2022年11月12日22:05:35
    版本:V 0.0.1
    邮箱:kevinlq@163.com
    版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
    希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->

---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**