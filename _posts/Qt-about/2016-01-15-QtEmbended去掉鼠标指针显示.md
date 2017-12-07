---
layout : post
title: QtEmbended去掉鼠标指针显示
category : Qt学习
wangyiyun: true
date : 2016-01-15
---

******

    作者:鹅卵石
    时间:2016年01月28日14:50:09
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

# QtEmbended去掉鼠标指针显示

搞过嵌入式的都往ARM板子上跑过自己的QT程序，当把程序移植到板子上时，发现总是存在一个鼠标指针，这显然不是很美观，怎么样去掉呢，其实也是有对应的办法的。很简单的一句代码就搞定了

打开主函数，添加以下代码即可

1. 引入头文件
```
#include <QWSServer>
```

2. 置鼠标样子属性为false
```
QWSServer::setCursorVisible(false);
```

QWSServer Class 为 Qt for Embeded Linux 封装了一个服务进程， 当你运行一个基于嵌入式平台的Qt程序时，它只能作为一个server或者连接到一个已经存在的server来运行

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)