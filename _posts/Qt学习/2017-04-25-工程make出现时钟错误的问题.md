---
layout : post
title: 工程make出现时钟错误的问题
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

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)