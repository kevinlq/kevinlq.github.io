---
layout : life
title : linux下用logroate进行日志切割
category : linux学习
duoshuo : true
date : 2016-03-09
---


******

* TOC

{:toc}

<!-- more -->

## 0x01 简介

官网介绍请看[这里](http://linuxcommand.org/man_pages/logrotate8.html)

The logrotate utility is designed to simplify the administration of log files on a system which generates a lot of log files. Logrotate allows for the automatic rotation compression, removal and mailing of log files. Logrotate can be set to handle a log file daily, weekly, monthly or when the log file gets to a certain size.

## 0x02 关键文件说明

以下是logrotate运行的关键点：

* `/usr/bin/logrotate` 程序所在位置；
* `/etc/cron.daily/logrotate` 默认让系统的cron每天执行logrotate一次；
* `/etc/logrotate.conf` 全局配置文件；
* `/etc/logrotate.d` 应用自个的配置文件存放目录，覆盖全局配置；

## 0x03 关键文件说明

**系统配置文件：**

```sh
    # see "man logrotate" for details
    # rotate log files weekly
    weekly
    
    # use the syslog group by default, since this is the owning group
    # of /var/log/syslog.
    su root syslog
    
    # keep 4 weeks worth of backlogs
    rotate 4
    
    # create new (empty) log files after rotating old ones
    create
    
    # uncomment this if you want your log files compressed
    #compress
    
    # packages drop log rotation information into this directory
    include /etc/logrotate.d
    
    # no packages own wtmp, or btmp -- we'll rotate them here
    /var/log/wtmp {
        missingok
        monthly
        create 0664 root utmp
        rotate 1
    }
    
    /var/log/btmp {
        missingok
        monthly
        create 0660 root utmp
        rotate 1
    }
    
    # system-specific logs may be configured here

```

**用户自定义的配置文件：**

保存在`/etc/logrotate.d/`下面，所以若是想要进行自己软件的日志切割则直接在这里进行编写自己的配置文件即可；

## 0x04 配置项说明

**nginx示例日志切割配置文件:**

```sh
/usr/local/nginx/logs/*.log {
    daily   
    dateext 
    rotate 1
    olddir  /usr/local/nginx/other/logs
    missingok  
    notifempty 
    compress 
    sharedscripts 
    postrotate
        /bin/kill -USR1 $(cat /var/run/nginx.pid 2>/dev/null) 2>/dev/null ||:
    endscript

}
```

**配置项说明：**

* dateext : 文件后缀是日期格式,也就是切割后文件是:xxx.log-20150529(.gz) 这样,如果注释掉,切割出来是按数字递增,即前面说的 xxx.log-1(.gz)
* compress : 通过gzip 压缩转储旧的日志
* nocompress : 不需要压缩时，用这个参数
* copytruncate : 用于还在打开中的日志文件，把当前日志备份并截断
* nocopytruncate : 备份日志文件但是不截断
* create mode owner group：使用指定的文件模式创建新的日志文件
* nocreate：不建立新的日志文件
* delaycompress：和 compress 一起使用时，转储的日志文件到下一次转储时才压缩
* nodelaycompress：覆盖 delaycompress 选项，转储同时压缩。
* errors address：专储时的错误信息发送到指定的Email 地址
* ifempty：即使是空文件也转储，这个是 logrotate 的缺省选项。
* notifempty：如果是空文件的话，不转储
* mail address：把转储的日志文件发送到指定的E-mail 地址
* nomail：转储时不发送日志文件
* olddir directory：转储后的日志文件放入指定的目录，必须和当前日志文件在同一个文件系统
* noolddir：转储后的日志文件和当前日志文件放在同一个目录下
* prerotate/endscript：在转储以前需要执行的命令可以放入这个对，这两个关键字必须单独成行
* postrotate/endscript：在转储以后需要执行的命令可以放入这个对，这两个关键字必须单独成行
* sharedscripts：所有的日志文件都轮转完毕后统一执行一次脚本
* monthly: 日志文件将按月轮循。其它可用值为‘daily’，‘weekly’或者‘yearly’。
* rotate count：指定日志文件删除之前转储的次数，0 指没有备份，5 指保留5 个备份
* size size：当日志文件到达指定的大小时才转储，Size 可以指定 bytes (缺省)以及KB (sizek)或者MB
* missingok: 在日志轮循期间，任何错误将被忽略，例如“文件无法找到”之类的错误。

## 0x05 手动命令说明

```sh
    用法: logrotate [OPTION...] <configfile>
      -d, --debug               Don't do anything, just test (implies -v)
      -f, --force               Force file rotation
      -m, --mail=command        Command to send mail (instead of `/usr/bin/mail')
      -s, --state=statefile     Path of state file
      -v, --verbose             Display messages during rotation
          --version             Display version information
    
    Help options:
      -?, --help                Show this help message
          --usage               Display brief usage message
```

**示例1：**

要调用为/etc/lograte.d/下配置的所有日志调用logrotate：

```sh
    logrotate /etc/logrotate.conf
```

**示例2：**

要为某个特定的配置调用logrotate

```sh
    logrotate /etc/logrotate.d/log-file
```

**示例3：**

使用-d选项以预演方式运行logrotate。要进行验证，不用实际轮循任何日志文件，可以模拟演练日志轮循并显示其输出。

```sh
    logrotate /etc/logrotate.d/log-file
```

**示例4：**

即使轮循条件没有满足，我们也可以通过使用-f选项来强制logrotate轮循日志文件，-v参数提供了详细的输出。

```sh
    logrotate -vf /etc/logrotate.d/log-file
```


## 0x06 简单示例：

**nginx.logrotate(error.log)**

```sh
    /usr/local/nginx/logs/error.log {
    dateext
    create
    #maxage 90
    rotate 60
    missingok
    sharedscripts
    postrotate
    /bin/kill -USR1 `cat /var/run/nginx.pid`
    endscript
    }
```

**php.logrotate(php-fpm.log、php-fpm.log.slow)**

```sh
    /usr/local/php/var/log/php-fpm.log /usr/local/php/var/log/php-fpm.log.slow {
    dateext
    create
    #maxage 90
    rotate 60
    missingok
    sharedscripts
    postrotate
    /bin/kill -USR1 `cat /var/run/php-fpm.pid`
    endscript
    }
```

**mysql.logrotate(slow-log、localhost.err)**

```sh
    /var/lib/mysql/slow-log /var/lib/mysql/localhost.err {
    dateext
    create
    # notifempty
    #maxage 90
    rotate 60
    missingok
    sharedscripts
    postrotate
    /usr/local/mysql/bin/mysqladmin -uroot -p'password' flush-logs
    endscript
    }
```



