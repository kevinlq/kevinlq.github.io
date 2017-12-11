---
layout : post
title: linux下安装Qt后无法加载帮助文档插件问题
category : Qt学习
wangyiyun: true
date : 2017-04-07
---

******

    作者:鹅卵石
    时间:2017年4月5日11:04:24
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

#  linux下安装Qt后无法加载帮助文档插件问题

## 问题描述

![无法加载问题](/res/img/blog/Qt学习/2017-04-07.png)


## 解决方案

出现这个问题的原因是新安装的系统缺少一些依赖库导致的，重新安装后打开就ok了

终端输入命令

```
sudo apt-get install libqt4-dev    
sudo apt-get install libgstreamer0.10-dev    
sudo apt-get install libgstreamer-plugins-base0.10-dev  
```

然后重新启动Qt5.4问题就解决了

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)