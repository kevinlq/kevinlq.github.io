---
layout: post
title: 记一次科学上网
categories: [工具使用]
description: 记一次科学上网
keywords: VPN,tools,vultr
---

以前一直买的别人的V-P-N，一般是能满足要求的，有时候还是不爽，查了下自己动手搭建也是很简单的，特记录下以备查看。

## 1. 什么是VPN
VPN(Virtual Private Network)--虚拟专用网！


## 2. SS和SSR

### 2.1 SS
SS的作者是clowwindy。自己为了FQ，写了`shadowsocks`,简称`ss`或者叫做`影梭`.
后来他觉得这个东西非常好用，速度快，而且不会被封锁，他就把源码共享在了github上，
然后就火了，但是后来作者被请去喝茶，删了代码，并且保证不再参与维护更新。现在这个好像是一个国外的大兄弟在维护。

### 2.2 SSR
在ss作者被喝茶之后，github上出现了一个叫breakwa11(破娃)的帐号，声称ss容易被防火墙检测到，
所以在混淆和协议方面做了改进，更加不容易被检测到，而且兼容ss，改进后的项目叫shadowsocks-R，
简称ssr，然后ss用户和ssr用户自然分成了两个派别，互相撕逼，直到前阵子，破娃被人肉出来，
无奈之下删除了ssr的代码，并且解散了所有相关群组.

### 2.3 原理
ss和ssr它的原理都是一样的，就是socks5代理，。socks代理只是简单的传递数据包，而不必关心是何种协议，
所以socks代理比其他应用层代理要快的多、

## 3. 服务器购买
搭建VPN必须有一个服务器，这样才可以。此时我们访问google就编程了如下流程。

- 没有VPN

![](/res/img/blog/tools/vpn/1.png)

- 有VPN后

![](/res/img/blog/tools/vpn/2.png)

可以看出来，在没有VPN之前，我们要访问`gooogle`总会被墙挡住！

Shadowsocks （ss）简单地说就是一个网络数据加密工具，通过它给我们的数据进行加密，
让防火墙无法检测出数据内容，从而顺利传输数据

---
这里选择`vultr`作为本次服务器最佳选择.

- vultr是按小时计费；
- 可以随时删除和新开通一个服务器，新的服务器就是新的IP(每次新开服务需要0.01美元);

### 购买地址

[点击这个链接有优惠!https://www.vultr.com/?ref=7409951](https://www.vultr.com/?ref=7409951)

进去之后直接注册就行

![](/res/img/blog/tools/vpn/3.png)

### 账号充值

![](/res/img/blog/tools/vpn/4.png)


### 创建服务

选择 "Servers" ，点击右边蓝色的"+" 添加 VPS。

![](/res/img/blog/tools/vpn/5.png)

- 节点选择

主要有以下几个节点，在选择是可以通过本地进行`ping`测试，看看那个延时小就选择哪个!

![](/res/img/blog/tools/vpn/6.png)

- 选择系统

系统这里有好几种，我选择的是默认的`CentOS`.

- 套餐选择

套餐这里选择默认的，也是最便宜的套餐!

![](/res/img/blog/tools/vpn/7.png)

- 其他选项

有 IPv6，私密，域名、标签等，无特殊需要全部默认即可。

选好后点击右下角的 “Deploy Now” ，开始安装 VPS。

## 查看VPS服务状态

![](/res/img/blog/tools/vpn/8.png)

## 搭建shadowsocks

以下使用windows进行连接服务器进行管理.

使用`X-Shell`连接刚才建立好的服务器.输入对应的用户名和密码进行登录验证。

### 安装BBR

BBR 为谷歌免费开源项目，加速优化！

```shell
# 下载脚本
wget --no-check-certificate https://github.com/teddysun/across/raw/master/bbr.sh
 
# 添加执行权限
chmod +x bbr.sh
 
# 执行脚本安装
./bbr.sh
```

提示你需要进行重新启动!

```
输入
lsmod | grep bbr
```

若出现如下提示，说明安装成功!

![](/res/img/blog/tools/vpn/9.png)

### 安装SS

依次执行下面三个命令即可，在选择端口是尽量大于1000

```
wget --no-check-certificate -O shadowsocks.sh https://raw.githubusercontent.com/teddysun/shadowsocks_install/master/shadowsocks.sh
 
chmod +x shadowsocks.sh
 
./shadowsocks.sh 2>&1 | tee shadowsocks.log
```
然后一路回车即可!

### 多端口配置

多端口多用户密码配置

```
vi /etc/shadowsocks/config.json
```
粘贴如下内容，将打开的内容全部替换(端口和对应的密码自己酌情配置)
```
{
"server":"0.0.0.0",
"local_address":"127.0.0.1",
"local_port":1080,
"port_password":{
"8989":"password8989",
"7979":"password7979",
"6969":"password6969"
},
"timeout":300,
"method":"aes-256-cfb",
"fast_open": false
}
```

修改完毕保存后进行重新启动

```
/etc/init.d/shadowsocks restart
```

### 常用的命令

```
启动：/etc/init.d/shadowsocks start 
停止：/etc/init.d/shadowsocks stop 
重启：/etc/init.d/shadowsocks restart 
状态：/etc/init.d/shadowsocks status
```

如果只能用第一个账号，其他2个账号都不能用，那是防火墙对另外的端口做了限制，此时可以让防火墙不拦截 7979和6969端口
 
```
查询端口
firewall-cmd --permanent --zone=public --list-ports

添加端口
firewall-cmd --zone=public --add-port=7979/tcp --permanent
firewall-cmd --reload

删除端口
firewall-cmd --zone=public --remove-port=6969/tcp --permanent

```
其他端口配置同上!

## 客户端配置

### windows客户端

- 下载地址；

[https://github.com/shadowsocks/shadowsocks-windows/releases](https://github.com/shadowsocks/shadowsocks-windows/releases)

打开软件后填写对应服务器的地址、端口号以及对应的密码即可!

![](/res/img/blog/tools/vpn/10.png)

***设置代理模式，选择PAC***

![](/res/img/blog/tools/vpn/12.png)


**PAC模式**:也就是智能分流模式，根据规则去匹配你访问的网站，仅加速国外网站，国内网站不受影响，非常智能

* 优点
>节省流量，国内网站依旧走本地网络，速度快，绝大部分国外网站都走代 理，速度也快

* 缺点
>少部分国外网站不走代 理，无法起到加速效果，甚至无法访问（解决方案：自己编辑PAC规则列表，将不走代理的国外网址加入进去，或者直接选择全局模式）



**全局模式**:国内、国外所有网站都走代理
* 优点
>可访问全球所有网站

* 缺点
>所有网站都走代 理，访问国内网站速度变慢，且耗流量


### android客户端

[https://github.com/shadowsocks/shadowsocks-android/releases](https://github.com/shadowsocks/shadowsocks-android/releases)



## 大功告成
![](/res/img/blog/tools/vpn/11.png)


## 其他一些设置

### 禁止root用户直接登录

### 禁用SSH 22端口，重新指定端口

开通第二天使用ssh登录主机时发现，系统提示之前登录失败的次数好多，每次都1000以上，有点担心了，所以直接关闭ssh 22端口，重新指定端口
重新定义新的端口吧，同时也新建用户名，禁止直接使用root用户进行登录!

```
再重新设置ssh端口和新建用户时先不要删除或者禁止之前的，以免配置失误无法登录的尴尬！

再确保新的配置生效后在禁用22端口以及root用户！
```

## 参考文章

* [http://www.hellojava.club/](http://www.hellojava.club/)
* [Vultr VPS搭建教程-新手](https://go2think.com/%E7%A7%91%E5%AD%A6%E4%B8%8A%E7%BD%91%EF%BC%9Avultr-vps-%E6%90%AD%E5%BB%BA-shadowsocks%EF%BC%88ss%EF%BC%89%E6%95%99%E7%A8%8B%EF%BC%88%E6%96%B0%E6%89%8B%E5%90%91%EF%BC%89/)
* [CentOS7 设置SSH服务以及端口修改](https://blog.csdn.net/mrqiang9001/article/details/78308830)

******

    作者:鹅卵石
    时间:2018年4月30日14:15:51
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**
