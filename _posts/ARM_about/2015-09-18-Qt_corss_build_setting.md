---
layout : post
title: Qt交叉编译后ARM板启动配置
category : ARM学习
wangyiyun: true
date : 2015-09-11
---

# Qt交叉编译后ARM板启动配置

本次移植完运行的程序界面

![demo](/res/img/blog/ARM学习/smarthome.png)



Qt编写好的程序在linux环境中交叉编译完，在开发板中的一些列环境变量的配置步骤如下

>涉及到的脚本见附录

##  1. 在开发板中建立工程目录

```
mkdir QtProgram
```

## 2. 添加环境变量

- 将启动脚本放入1中建立的目录中，即start_qt.sh
- 将qt4-dev脚本放入/etc目录中，并加入到系统的环境变量中来

![Tiny4412](/res/img/blog/ARM学习/arm.png)

添加命令

![Tiny4412](/res/img/blog/ARM学习/arm_add_order.png)

完成上述命令后重新西东系统(执行reboot即可)

- 解压从ubuntu编译完成的动态库，按照默认解压路径即可

解压后会看到如下结构

![动态库结构](/res/img/blog/ARM学习/arm_dis.png)

- 启动自己的Qt程序

进入1中建立的目录，只需要执行启动脚本即可

![启动Qt程序](/res/img/blog/ARM学习/arm_qt_start.png)

- 设置开机启动配置文件qt4.d拷贝到如下目录中:

![](/res/img/blog/ARM学习/arm_qt4.png)


![](/res/img/blog/ARM学习/arm_qt42.png)


在打开的文件末尾添加如下启动配置，即可开机启动，这里关闭了友善支臂自带的qtopia系统，为了使该系统和自己的Qt程序触摸产生影响，故这里禁止启动

![开机启动](/res/img/blog/ARM学习/qt_start.png)

在qt4.d脚本文件最后，是自己程序启动的脚本

![开机启动](/res/img/blog/ARM学习/qt_start2.png)


Qt程序存放目录结构

![工程目录](/res/img/blog/ARM学习/qtprogram.png)

##  3. 一些启动脚本

qt4_env
```
#!/bin/sh

if [ -c /dev/input/event0 ]; then
    INPUT_PATH=/dev/input
else
    INPUT_PATH=/dev
fi
export TSLIB_TSDEVICE=/dev/input/event1
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_PLUGINDIR=/lib/ts
export TSLIB_CALIBFILE=/etc/pointercal
export QTDIR=/usr/local/Trolltech/QtEmbedded-4.8.6-arm/
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib
export QWS_MOUSE_PROTO="Tslib:$INPUT_PATH/event1"
#export QWS_MOUSE_PROTO="Tslib:$INPUT_PATH/event1 MouseMan:$INPUT_PATH/mouse1"
export QWS_KEYBOARD=TTY:/dev/tty1

#sync font size with QtDesigner 
export QWS_DISPLAY="LinuxFB:mmWidth200:0"
#export QWS_SIZE="480x272"

export KDEDIR=/opt/kde

export HOME=/root

```


qt4.d

```
#!/bin/sh
source /etc/qt4_env
if [ -f etc/pointercal ];then
    echo "use exist pointercal file ..."
else
    echo "touch screen to create pointercal file ..."
    /bin/ts_calibrate
fi

# if you want run qt4 app at booting system,just add the command line below

```

start_qt.sh

```
#!/bin/sh

if [ -e /etc/friendlyarm-ts-input.conf ] ; then
	. /etc/friendlyarm-ts-input.conf
fi
true	${TSLIB_TSDEVICE:=/dev/touchscreen}

if grep -Ei "\<ctp=(1|2|3)\>" /proc/cmdline >/dev/null; then
	TSLIB_CONFFILE=/etc/ts-mt.conf
	echo "-27 13757 -2071768 -9105 -32 34531248 65536 800 480" \
		>/etc/pointercal
	sync
else
	TSLIB_CONFFILE=/etc/ts.conf
fi

export TSLIB_TSDEVICE
export TSLIB_CONFFILE

export TSLIB_PLUGINDIR=/usr/lib/ts
export TSLIB_CALIBFILE=/etc/pointercal

export QWS_DISPLAY=:1
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
export PATH=/bin:/sbin:/usr/bin/:/usr/sbin:/usr/local/bin

export QWS_MOUSE_PROTO="Tslib MouseMan:/dev/input/mice"
if [ -c ${TSLIB_TSDEVICE} ]; then
	if [ ! -s /etc/pointercal ] ; then
		rm /etc/pointercal
		/usr/bin/ts_calibrate
	fi
else
	export QWS_MOUSE_PROTO="MouseMan:/dev/input/mice"
fi

export QTAPP=$PWD
export QTDIR=/usr/local/Trolltech/QtEmbedded-4.8.6-arm/
export PATH=$QRDIR/bin:$PATH
export LD_LIBRARY=$QTDIR/lib
export QWS_DISPLAY="LinuxFB:mmWidth200:0"

export QWS_KEYBOARD=TTY:/dev/tty1
export KDEDIR=/opt/kde
export HOME=/root
/QtProgram/WifiCarV2 -qws

```

参考文章地址：(guoyifei)

[参考地址](http://bbs.qter.org/forum.php?mod=viewthread&tid=2476)




******

    作者:鹅卵石
    时间:2014年9月11日22:16:01
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="/res/img/myCode.png" width="80%" height="80%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---

