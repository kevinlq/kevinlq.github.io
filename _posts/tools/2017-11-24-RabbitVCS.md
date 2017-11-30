---
layout: post
title: Ubuntu下SVN客户端(RabbitVCS)
categories: [工具使用, SVN]
description: Ubuntu下SVN使用客户端进行相关操作
keywords: Ubunt, SVN
---

## RabbitVCS

`RabbitVCS`是一款Linux下的类似Windows下的Tortoisesvn，使用起来还是很方便的。尤其是在log以及diff的时候。

## 安装
1. sudo 安装
```
sudo add-apt-repository ppa:rabbitvcs/ppa
```
> 将rabbitvcs的添加到源里面。操作会提示是否要添加到源里面，点击ENTER添加，ctrl + c不添加

2. 导入key
```
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 34EF4A35
```

3. 执行下更新
```
sudo apt-get update
```

4. 安装依赖
```
sudo apt-get install rabbitvcs-cli  rabbitvcs-core rabbitvcs-gedit
rabbitvcs-nautilus rabbitvcs-thunar thunarx-python
上面是一条命令
```

**注意:**
> 安装成功之后，需要注销，然后重新登录后才能看到哦.

## 使用
`RabbitVCS`的使用了在Windows下使用`Tortoisesv`类似，操作步骤基本一致，都可以通过单击鼠标右键进行操作.

![RabbitVCS](/res/img/blog/tools/rabbitvcs.png)

简单容易上手!
