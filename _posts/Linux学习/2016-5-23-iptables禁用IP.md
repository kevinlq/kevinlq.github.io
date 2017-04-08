---
layout : life
title: iptables禁用IP
category : linux学习
duoshuo: true
date : 2016-5-23
---


    作者: daodaoliang
    时间: 2016-5-23
    版本: V0.0.1
    邮箱: daodaoliang@yeah.net

<!-- more -->

******

### 0x01 确保服务启动

```sh
    service iptables status
    /etc/init.d/iptables stop
    /etc/init.d/iptables start
```

### 0x02 禁用IP

**禁用单个IP:**

```sh
    iptables -I INPUT -s 114.113.112.111 -j DROP
```

**禁用IP段的命令:**

```sh
    iptables -I INPUT -s 114.113.0.0/16 -j DROP
    iptables -I INPUT -s 114.112.0.0/16 -j DROP
    iptables -I INPUT -s 114.111.1.0/24 -j DROP
    iptables -I INPUT -s 114.111.2.0/24 -j DROP
```

**禁用整个IP段的命令:**

```sh
    iptables -I INPUT -s 114.0.0.0/8 -j DROP
```

### 0x03 服务器自启动

* 把它加到/etc/rc.local中
* `iptables-save >/etc/sysconfig/iptables` 可以把你当前的 iptables 规则放到 `/etc/sysconfig/iptables` 中，系统启动 iptables 时自动执行。
* `service   iptables   save` 也可以把你当前的 iptables 规则放 `/etc/sysconfig/iptables` 中，系统启动 iptables 时自动执行。

### 0x04 解封IP

* iptables -L --line-numbers 然后iptables -D INPUT 序号

### 0x04 开启端口

```sh
    iptables -I INPUT -p tcp --dport 80 -j ACCEPT
    iptables -I INPUT -p tcp --dport 22 -j ACCEPT
```

```sh
    /etc/rc.d/init.d/iptables save
    or
    iptables-save
```

### 0x05 关闭端口

```sh
    iptables -I INPUT -p tcp --dport 80 -j DROP
    iptables -I INPUT -p tcp --dport 22 -j DROP
```

```sh
    /etc/rc.d/init.d/iptables save
    or
    iptables-save
```

### 0x06 简单例子

````sh
    *filter
    :INPUT ACCEPT [0:0]
    :FORWARD ACCEPT [0:0]
    :OUTPUT ACCEPT [0:0]
    -A INPUT -s 49.70.70.38/32 -j DROP
    -A INPUT -s 112.232.74.87/32 -j DROP
    -A INPUT -i lo -j ACCEPT
    -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 22 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 80 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 443 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 3000 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 5000 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 1022 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 5555 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 5556 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 10022 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 3306 -j ACCEPT
    -A INPUT -p tcp -m state --state NEW -m tcp --dport 3013 -j ACCEPT
    -A INPUT -p icmp -m icmp --icmp-type 8 -j ACCEPT
    -A INPUT -j REJECT --reject-with icmp-port-unreachable
    -A FORWARD -j REJECT --reject-with icmp-port-unreachable
    -A OUTPUT -o lo -j ACCEPT
    -A OUTPUT -j ACCEPT
    COMMIT
```






