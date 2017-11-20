---
layout : post
title: ARM平台下利用无线网卡自组网实现三跳通信-AODV协议
category : ARM学习
wangyiyun: true
date : 2016-10-04
---

******

    作者:鹅卵石
    时间:2016年10月04日18:38:09
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

# ARM平台下利用无线网卡自组网实现三跳通信-AODV协议

## 介绍

ARM平台下无线通信一般都是点对点的，这次需求是要实现无线网卡的三跳，即实现A主机发送信息通过B主机中转后在发送给C，不能是直接从A到C，这里就涉及到了一个最基本的路由转发协议。其实早就有了开源的协议-AODV协议，该协议已经能够完美的解决我的需求了

AODV(Ad hoc On-demand Distance Vector Routing)是一种按需路由协议。实现细节这里不做过多的介绍，后期慢慢研究源代码

## 硬件平台环境

* 目标平台：Tiny4412,Linux 3.5.0 
* 宿主机：ubuntu14.04 
* 工具链：arm-linux-gcc-4.5.1 
* Aodv版本：AODV-UU-0.9.6

## 编译

### linux内核配置

由于aodv协议是和linux内核的netfilter模块进行通信的，所以首先必须确保你的linux内核当初编译的时候包含了此模块，否则这里也是编译不过去的，为了确保没有错误还是再次配置下内核模块

解压源码后切换到源码根目录执行命令

```
make menuconfig
```

接着就会启动图形化的配置界面，按照以下部分进行勾选

>Networking support —> Networking options —> 
Networking packet filtering framework (Netfilter) —> 
IP: Netfilter configuration —> 
<*> IP Userspace queueing via NETLINK (OBSOLETE)

配置完成后编译内核，如果你的内核之前已经编译好了，没有必要全部编译一次，只需要按模块编译即可

```
make modules 
```

编译完成后会生成相应的这个模块

### aodvy源码修改

下载aodv源码并解压，进入到根目录，打开Makefile文件进行修改：

![aodv源码修改](/res/img/blog/ARM学习/aodv_source.png)

打开lkx/kaodv-debug.h文件进行修改： 

![aodv源码修改](/res/img/blog/ARM学习/aodv_source2.png)

打开lnx/kaodv-netlink.c文件进行修改： 

![aodv源码修改](/res/img/blog/ARM学习/aodv_source3.png)

打开lnx/Makefile进行修改： 

![aodv源码修改](/res/img/blog/ARM学习/aodv_source4.png)


### 编译

以上部分就是关于aodv协议的一些配置，需要修改的地方都用红色圈起来了。先忙切换到源码主目录进行编译：

```
make arm
```

编译完成后会生成2个文件：aodvd和kaodv.ko文件，如下图所示：

![编译aodv](/res/img/blog/ARM学习/aodv_make.png)

## 运行

### 网卡配置

```
ifconfig wlan0 down 
iwconfig wlan0 mode ad-hoc essid “wifi-adhoc” 
iwconfig wlan0 channel 10 
ifconfig wlan0 192.168.3.101 up
```


### 模块加载

![加载aodv](/res/img/blog/ARM学习/aodv_load.png)

## 测试

将生成的2个文件拷贝到开发板上后将无线网卡先配置成ad-hoc模式，设置IP地址，使的都在同一个网段内。我这里使用衰减器，使开发板A和开发板C不通，A和B能通，B和C能通，然后各自运行aodv协议，在开发板ApingC就能ping通，查看A的路由表确实已经改变了：

![测试aodv](/res/img/blog/ARM学习/aodv_test.png)


---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)