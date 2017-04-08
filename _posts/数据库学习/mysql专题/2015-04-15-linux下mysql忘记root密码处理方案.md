---
layout: life
title: linux下mysql忘记root密码的处理方案
category: 数据库学习
duoshuo: true
date: 2015-05-17
---


#### 准备工作

* 首先确认mysql数据库处于安全状态，因为在找回密码的过程中，mysql处于没有密码保护的状态，任何人都可以连接数据库并且对数据库作出修改，建议断开对外端口，或者断开对外网线，最保险的办法就是断开对外网络同时登录服务器通过控制台操作
* 

<!-- more -->


#### 修改mysql的登录设置

* 修改配置文件

```sh
    vi /etc/my.cnf 
```

在```mysqld```标签下加入下面这句

> skip-grant-tables 

* 重新启动mysqld

```sh
    /etc/init.d/mysqld restart 
```

* 登录并修改mysql密码

```sh
    /usr/bin/mysql 
    use mysql;
    update user set password=PASSWORD('p12#456') where user='root' and host='root' or host='localhost';
    flush privileges;
```

* 修改配置文件并重启服务

将刚才在[mysqld]的段中加上的skip-grant-tables删除 


```sh
     /etc/init.d/mysqld restart 
```
