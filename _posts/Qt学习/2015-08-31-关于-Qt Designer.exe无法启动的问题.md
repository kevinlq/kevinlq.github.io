---
layout : post
title: 关于Qt Designer.exe无法启动的问题
category : Qt学习
wangyiyun: true
date : 2015-08-31
---

******

	作者:鹅卵石
    时间:2015年08月31日10:10:09
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

## 关于Qt Designer.exe无法启动的问题
使用Qt有一段时间了，一直好好的，今天突然发现**Designer.exe**无法打开了，不管是在Qt Creater中还是直接双击Designer.exe都无法运行，显示结果是：
![这里写图片描述](/res/img/blog/Qt学习/1.png)
郁闷了好几个小时，当时怀疑自己电脑坏了，把Qt卸载了重新安装，可是结果还是一样，又再次怀疑自己没有卸载干净，再次卸载，安装，依然不起作用，到最后都怀疑自己电脑系统坏了，打算重新装系统呢。作为一个IT人士，装不起啊，要安装的软件太多了，无意间在网上找到的方案，试了下果然可以打开了，在此记录下，以免以后遇到类似的问题，也献给遇到和我相同问题的朋友。

----------

### 首先说说解决方案，打开以下路径：
![这里写图片描述](/res/img/blog/Qt学习/2.png)
找到上图中所示的路劲，删除红色标记的文件即可。这个.xml文件是Qt Desinder的配置文件，出现打不开的问题我想就是启动配置出现了问题了。
参考地址：[http://bbs.csdn.net/topics/390304302#quote](http://bbs.csdn.net/topics/390304302#quote)

顺便晒下刚完成的一个上位机软件。只晒图贴代码，想要的可以私聊。
![这里写图片描述](/res/img/blog/Qt学习/3.png)

***

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)