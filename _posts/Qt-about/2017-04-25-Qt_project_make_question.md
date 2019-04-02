---
layout : post
title: Qt工程make出现时钟错误的问题
category : Qt学习
wangyiyun: true
date : 2017-04-25
---

******

    作者:鹅卵石
    时间:2017年4月25日16:11:45
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

##  工程make出现时钟错误的问题

### 问题描述
将本机工程文件拷贝到其他电脑进行编译时出现的时钟问题

![错误描述](/res/img/blog/Qt学习/2017-04-25 15-46-58.png)

### 解决方法

出现这个问题的原因是时间不一致导致的，也就是说某些文件修改的时间比当前系统时间晚

直接修改文件时间
```
touch xxx.h
```

当然也可以直接修改整个工程的时间
```
touch *
```

修改完成后重新编译没有问题

## 安装 Qt Creator后出现的问题

### 问题描述

``
Time: 2019年4月2日22:24:45
``

在W10电脑上，安装Qt5.11之后的几个版本，在 Qt Creator 启动后创建一个简单工程，控制台错误输出如下：

```
系统找不到指定的文件。
Project ERROR: Cannot run compiler 'g++'. Output:
===================
===================
Maybe you forgot to setup the environment?
Error while parsing file D:\Project\untitled2\untitled2.pro. Giving up.
```

只有这台电脑有问题，其他电脑没有任何问题，很奇怪！！！

错误信息如下图所示：

![](/res/img/blog/Qt-learn/Qt-question/console-out.jpg)

套件配置信息如下，均配置正确

![](/res/img/blog/Qt-learn/Qt-question/kit.jpg)

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)