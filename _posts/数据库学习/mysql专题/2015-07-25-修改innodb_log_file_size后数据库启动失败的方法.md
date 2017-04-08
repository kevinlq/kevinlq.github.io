---
layout : life
title: mysql修改my.cnf的innodb_log_file_size后数据库启动失败的解决方法
category : 数据库学习
duoshuo: true
date : 2015-07-24
---

<!-- more -->


```sh
# 找到mysql的数据目录下的ib_logfile文件，删除，然后重启mysql
# 数据文件在配置文件有配置，若是默认则在`/var/lib/mysql/`下面
```

