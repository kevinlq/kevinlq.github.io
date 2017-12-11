---
layout : post
title: cJson学习使用
category : json学习
wangyiyun: true
date : 2017-07-19
---

******

    作者:devstone
    时间:2017年7月19日17:30:09
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net

<!-- more -->

## 概述

cJson是一个轻巧、方便、单文件的ANSI-C标准的JSON解析器.


## 下载

直接在这里进行下载即可

[下载地址github](https://github.com/DaveGamble/cJSON)

下载后可以看到主要有2个文件构成:
```C
cJSON.h
cJSON.c
```
- 其中在.h文件中包含了常见的数据类型定义以及cJSON结构体声明，还有一些函数声明.   
- 在.c中主要是一些函数的实现.

## 使用

- cJson结构体

```c
typedef struct cJSON {
       struct cJSON*next,*prev;    /* 遍历数组或对象链的前向或后向链表指针*/
       struct cJSON *child;        /*数组或对象的孩子节点*/
       int type;                   /* key的类型*/
       char *valuestring;          /*字符串值*/
       int valueint;               /* 整数值*/
       double valuedouble;         /* 浮点数值*/
       char *string;               /* key的名字*/
} cJSON;
```
可以看出:
1. cJson结构体中包含了一个双向连接，可以通过child指针访问下一层;
2. type类型决定了数据类型;
3. valuestring、valueint和valuedouble就是具体的数据类型;
4. string为具体节点的名称

- boolean值定义

我们知道C中是没有bool类型的，cJSON为了跨平台做了一个特殊处理:

```C
#define true ((cJSON_bool)1)
#define false ((cJSON_bool)0)
```

cJSON_bool 是自定义的一个类型,在cJSON头文件中
```C
typedef int cJSON_bool;
```

## 栗子

.......

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)
