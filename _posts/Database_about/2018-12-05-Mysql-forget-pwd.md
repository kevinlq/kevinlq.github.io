---
layout: post
title: MySql 数据库找回密码
categories: [数据库学习]
description: 忘记MySql密码
keywords: MySql
---


自己电脑上的 `Mysql` 数据库好长时间不用，竟然忘记密码了，记录下过程，以备再次忘记如何找回或修改.

## Windows 下处理

- 关闭 `MySql` 服务

启动命令行，最好用管理员权限启动，执行以下命令:

```
net stop mysql
```

- 跳过服务权限认证

直接在命令行执行下面命令，前提是已经将 `MySql` 配置到系统环境变量中来:

```
mysqld -nt --skip-grant-tables
```

如果上面提示，`mysqld` 命令找不到，则是没有配置系统环境变量，需要使用 `cd` 命令到 `MySql` 安装路径下: `C:\Program Files (x86)\MySQL\MySQL Server 5.5\bin`
接着执行上面的命令.

可以会出现这样的警告提示, 无须理会:

```
181205 22:50:00 [Warning] option 'new': boolean value 't' wasn't recognized. Set to OFF.
```



- 进入 `MySql`

使用管理员权限再次打开另一个命令窗口，执行下面的命令:

![](/res/img/blog/DataBase_About/mysql_modifypwd02.png)

```
select host,user,password from mysql.user;
```

即可显示当前所有的用户名和对应的密码，一般看到的密码都是加密过的，需要进行修改

- 修改 `root` 密码

执行如下命令

```
use mysql;

update user set password= 123456 where user="root";

flush privileges;	//刷新
```

上面设置 `root` 密码为 123456


- 重新登录查看

关闭上面的所有 `CMD` 命令窗口，再次进入 `MySql`:

![](/res/img/blog/DataBase_About/mysql_userPwd.png)

可以看到密码被修改了!




******

    作者:鹅卵石
    时间: 2018年12月5日22:27:55
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="/res/img/myCode.png" width="50%" height="50%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---
