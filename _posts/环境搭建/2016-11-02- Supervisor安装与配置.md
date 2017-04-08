---
layout : life
title : supervisor 安装与配置
category : 环境搭建
duoshuo : true
date : 2016-11-02
---


******
	作者: daodaoliang
    版本: V1.0.1
    邮箱： daodaoliang@yeah.net

[这里是官方链接][1]

<!-- more -->

## 0x00 supervisor 简介

Supervisor 是一个用 python 语言开发的用来在 linux 系统下进行进程管理的工具。可以很方面的监控、启动、停止、重启一个或者多个进程。当进程意外终止时可以在第一时间进行重启，做到自动恢复。

## 0x01 supervisor 安装

**1.确保 python 环境完备**

**Note:** Supervisor is known to work with Python 2.4 or later but will not work under any version of Python 3.

安装 [easy_install][2] 或者 [pip][3]：

```
	wget --no-check-certificate https://bootstrap.pypa.io/ez_setup.py -O - | sudo python
```

或者

```
	# get-pip.py请从官网获取
	python get-pip.py
```

**2.安装 supervisor**

```
	pip install supervisor
```

或者

```
	easy_install supervisor
```

或者

```
	# 需要源支持
	yum install supervisor
```

supervisor安装完成后会生成三个执行程序：supervisortd、supervisorctl、echo_supervisord_conf，分别是supervisor的守护进程服务（用于接收进程管理命令）、客户端（用于和守护进程通信，发送管理进程的指令）、生成初始配置文件程序。

## 0x02 supervisor 配置

**1. 配置文件生成**

运行supervisord服务的时候，需要用 c 参数指定supervisor配置文件，如果没有显示指定，默认以此在以下目录查找：

+ `$CWD/supervisord.conf`
+ `$CWD/etc/supervisord.conf`
+ `/etc/supervisord.conf`
+ `/etc/supervisor/supervisord.conf (since Supervisor 3.3.0)`
+ `../etc/supervisord.conf (Relative to the executable)`
+ `../supervisord.conf (Relative to the executable)`

**Note：** `$CWD` 表示运行 `supervisord` 程序的目录。

可以通过运行 `echo_supervisord_conf` 程序生成 `supervisor` 的初始化配置文件,如下所示:

```
	mkdir /etc/supervisor
    echo_supervisord_conf > /etc/supervisor/supervisord.conf
```

**2. 配置项说明**

supervisor的配置参数较多，下面介绍一下常用的参数配置，详细的配置及说明，请参考[官方文档][4]介绍。分号（;）开头的配置表示注释

```ini

[unix_http_server]

;socket文件的路径，supervisorctl用XML_RPC和supervisord通信就是通过它进行的,不设置的话，默认为none。非必须设置。
file=/tmp/supervisor.sock

;修改file的权限必须为0700,非必须设置。
chmod=0700

;修改file的权限的所属组为nobody:nogroup,非必需设置。
chown=nobody:nogroup

;使用supervisorctl链接的时候,认证的用户,不设置的话默认不需要用户,非必需设置。
username=user

;和上面的用户名对应的密码，可以直接使用明码，也可以使用SHA加密如{SHA}82ab876d1387bfafe46cc1c8a2ef074eae50cb1d。
password=123

;侦听在TCP上的socket,Web Server和远程的supervisorctl都要用到他不设置的话，默认为不开启。非必须设置。
[inet_http_server]

;侦听的IP和端口，侦听所有IP用 :9001或*:9001或者0.0.0.0:9001.这个必须设置，只要上面的[inet_http_server]开启了，就必须设置它。
port=127.0.0.1:9001

;登录时的用户名。
username=user

;登录时的密码。
password=123

;定义supervisord这个服务端进程的一些参数的。
[supervisord]

;supervisord这个主进程的日志路径,默认路径$CWD/supervisord.log,$CWD是当前目录,非必需设置。
logfile=/tmp/supervisord.log

;这个是上面那个日志文件的最大的大小,当超过50M(默认值是50M)的时候,会生成一个新的日志文件。当设置为0时,表示不限制文件大小,非必须设置。
logfile_maxbytes=50MB

;日志文件保持的数量,supervisor在启动程序时,会自动创建10个buckup文件(默认10),用于log rotate当设置为0时,表示不限制文件的数量。
logfile_backups=10

;日志级别，有critical, error, warn, info, debug, trace, or blather等默认为info,非必须设置项。
loglevel=info

;supervisord的pid文件路径,默认为$CWD/supervisord.pid。
pidfile=/tmp/supervisord.pid

;如果是true,supervisord进程将在前台运行默认为false,也就是后台以守护进程运行。
nodaemon=false

;这个是最少系统空闲的文件描述符，低于这个值(默认值1024)supervisor将不会启动。系统的文件描述符在这里设置cat /proc/sys/fs/file-max。
minfds=1024

;最小可用的进程描述符,低于这个值(默认200)supervisor也将不会正常启动。ulimit  -u这个命令，可以查看linux下面用户的最大进程数。
minprocs=200

;进程创建文件的掩码,默认为022
umask=022

;这个参数可以设置一个非root用户，当我们以root用户启动supervisord之后。我这里面设置的这个用户，也可以对supervisord进行管理,默认不设置。
user=daodaoliang

;supervisord的标识符，主要是给XML_RPC用的。当你有多个supervisor的时候，而且想调用XML_RPC统一管理，就需要为每个supervisor设置不同的标识符了,默认sipervisor。
identifier=supervisor

;当supervisord作为守护进程运行的时候,设置这个参数的话,启动supervisord进程之前,会先切换到这个目录默认不设置。
directory=/tmp

;在supervisord进程启动的时候，把以前子进程产生的日志文件(路径为AUTO的情况下)清除掉,默认false,有调试需求的同学可以设置为true。
nocleanup=true

;当子进程日志路径为AUTO的时候，子进程日志文件的存放路径。默认路径是这个东西，执行下面的这个命令看看就OK了，处理的东西就默认路径
;python -c "import tempfile;print tempfile.gettempdir()"
childlogdir=/tmp

;设置环境变量的，supervisord在linux中启动默认继承了linux的环境变量，在这里可以设置supervisord进程特有的其他环境变量。
;supervisord启动子进程时，子进程会拷贝父进程的内存空间内容。 所以设置的这些环境变量也会被子进程继承。
;小例子：environment=name="daodaoliang",age="26"
environment=KEY="value"

;如果设置为true，会清除子进程日志中的所有 ANSI 序列（\n,\t等）。
strip_ansi=false

[rpcinterface:supervisor]

supervisor.rpcinterface_factory = supervisor.rpcinterface:make_main_rpcinterface

;针对supervisorctl的一些配置
[supervisorctl]

;supervisorctl本地连接supervisord的时候，本地UNIX socket路径，注意这个是和前面的[unix_http_server]对应的默认值就是unix:///tmp/supervisor.sock
serverurl=unix:///tmp/supervisor.sock

;supervisorctl远程连接supervisord的时候，用到的TCP socket路径,注意这个和前面的[inet_http_server]对应默认就是http://127.0.0.1:9001
serverurl=http://127.0.0.1:9001

;用户名
username=daodaoliang

;密码
password=123

;输入用户名密码时候的提示符
prompt=iamnami

;这个参数和shell中的history类似，我们可以用上下键来查找前面执行过的命令默认是no file,所以我们想要有这种功能，必须指定一个文件
history_file=~/.sc_history

;要管理的子进程了,":"后面的是名字，最好别乱写和实际进程有点关联最好。这样的program我们可以设置一个或多个，一个program就是要被管理的一个进程
[program:theprogramname]

;要启动进程的命令路径了，可以带参数/home/test.py -a 'hehe'
;需要注意的是，我们的command只能是那种在终端运行的进程，不能是守护进程。这个是个必须设置的项。
command=/bin/cat

;当numprocs为1时,process_name=%(program_name)s
;当numprocs>=2时,%(program_name)s_%(process_num)02d
process_name=%(program_name)s

;进程数量,当不为1时，就是进程池的概念
numprocs=1

;进程开始的数字
numprocs_start=1

;进程运行前，会前切换到这个目录
directory=/tmp

;进程掩码:--- -w- -w-, 转换后rwx r-x r-x
umask=022

;优先级,值越高,最后启动,最先被关闭,默认值999
priority=999

;如果为true则程序在supervisord启动后将会启动
autostart=true

;自动重启的情况
;有三个选项，false,unexpected和true。
;如果为false的时候，无论什么情况下，都不会被重新启动。
;如果为unexpected，只有当进程的退出码不在下面的exitcodes里面定义的退出码的时候，才会被自动重启。
;当为true的时候，只要子进程挂掉，将会被无条件的重启。
autorestart=true

;子进程气动阀多少秒后才把状态置为running
startsecs=1

;当进程启动失败后，最大尝试启动的次数。。当超过3次后，supervisor将把此进程的状态置为FAIL。
startretries=3

;和上面的的autorestart=unexpected对应。exitcodes里面的定义的退出码是expected的。
exitcodes=0,2

;进程停止信号，可以为TERM, HUP, INT, QUIT, KILL, USR1, or USR2等信号。当用设定的信号去干掉进程，退出码会被认为是expected。
;中断:INT(类似于Ctrl+C)(kill -INT pid),退出后会将写文件或日志(推荐)
;终止:TERM(kill -TERM pid)
;挂起:HUP(kill -HUP pid),注意与Ctrl+Z/kill -stop pid不同
;从容停止:QUIT(kill -QUIT pid) ;KILL, USR1, USR2其他见命令(kill -l),说明1
;默认为TERM。
stopsignal=TERM

;当我们向子进程发送stopsignal信号后，到系统返回信息给supervisord，所等待的最大时间。 超过这个时间，supervisord会向该子进程发送一个强制kill的信号。
;默认为10秒,非必须设置。
stopwaitsecs=10

;用于supervisord管理的子进程，这个子进程本身还有子进程。那么我们如果仅仅干掉supervisord的子进程的话，子进程的子进程有可能会变成孤儿进程.
;所以可以设置可个选项，把整个该子进程的整个进程组都干掉。 设置为true的话，一般killasgroup也会被设置为true。
;需要注意的是，该选项发送的是stop信号默认为false。
;非必须设置。
stopasgroup=false

;这个和上面的stopasgroup类似，不过发送的是kill信号。
;非必须设置
killasgroup=false

;如果supervisord是root启动，我们在这里设置这个非root用户，可以用来管理该program。
;非必须设置项。
user=daodaoliang

;如果为true，则stderr的日志会被写入stdout日志文件中
redirect_stderr=true

;子进程的stdout的日志路径，可以指定路径，AUTO，none等三个选项
;设置为none的话，将没有日志产生。
;设置为AUTO的话，将随机找一个地方生成日志文件,而且当supervisord重新启动的时候,以前的日志文件会被清空。
;当 redirect_stderr=true的时候，sterr也会写进这个日志文件。
stdout_logfile=/a/path

;日志文件最大大小，和[supervisord]中定义的一样。默认为50
stdout_logfile_maxbytes=1MB

;和[supervisord]定义的一样。默认10
stdout_logfile_backups=10

;这个东西是设定capture管道的大小，当值不为0的时候，子进程可以从stdout发送信息，而supervisor可以根据信息，发送相应的event。
;默认为0，为0的时候表达关闭管道。
;非必须项
stdout_capture_maxbytes=1MB

;当设置为ture的时候,当子进程由stdout向文件描述符中写日志的时候,将触发supervisord发送PROCESS_LOG_STDOUT类型的event.
;默认为false
;非必须设置
stdout_events_enabled=false

;这个东西是设置stderr写的日志路径，当redirect_stderr=true。这个就不用设置了，设置了也是白搭。因为它会被写入stdout_logfile的同一个文件中默认为AUTO。
;也就是随便找个地存,supervisord重启被清空。
;非必须设置
stderr_logfile=/a/path

;这个是该子进程的环境变量，和别的子进程是不共享的
environment=A="1",B="2"

serverurl=AUTO

;当我们要管理的进程很多的时候，写在一个文件里面就有点大了。我们可以把配置信息写到多个文件中，然后include过来。
[include]

;例如/an/absolute/filename.conf /an/absolute/*.conf foo.conf config??.conf
files = relative/directory/*.ini

```

**3.例子**

```ini
[program:usercenter]
; 程序的启动目录
directory = /home/leon/projects/usercenter
; 启动命令，可以看出与手动在命令行启动的命令是一样的
command = gunicorn -c gunicorn.py --port=%(process_num)s wsgi:app
; 程序名字
process_name=%(program_name)s_%(process_num)02d
; 开始数字
numprocs_start=8110
; 数量
numprocs=4
; 在 supervisord 启动的时候也自动启动
autostart = true
; 启动 5 秒后没有异常退出，就当作已经正常启动了
startsecs = 5
; 程序异常退出后自动重启
autorestart = true
; 启动失败自动重试次数，默认是 3
startretries = 3
; 用哪个用户启动
user = leon
; 把 stderr 重定向到 stdout，默认 false
redirect_stderr = true
; stdout 日志文件大小，默认 50MB
stdout_logfile_maxbytes = 20MB
; stdout 日志文件备份数
stdout_logfile_backups = 20
; stdout 日志文件，需要注意当指定目录不存在时无法正常启动，所以需要手动创建目录（supervisord 会自动创建日志文件）
stdout_logfile = /data/logs/usercenter_stdout.log
```

## 0x03 supervisord 启动和维护

**1.启动服务**

```
	/etc/init.d/supervisord -c /etc/supervisor.conf
```

**2.启动管理端**

```
	supervisorctl -c /etc/supervisor.conf
```

若是连接远程的则:

```
	supervisorctl -c /etc/supervisor.conf -s "http://114.114.114.114:9001"
```

**3.管理服务**

```
	# 查看服务状态
	status

    # 关闭 usercenter 程序
    stop usercenter

    # 停止所有程序
    stop all

	# 启动 usercenter 程序
	start usercenter

	# 启动所有程序
    start all

    # 重启 usercenter 程序
    restart usercenter

	# 重启所有程序
    restart all

	# 读取有更新的的配置文件，不会启动新程序
    reread

    # 重启启动远程supervisord
    reload

    # 更新配置文件，若有新的则启动，若删除了则停止
    update
```


[1]:http://supervisord.org/installing.html
[2]:https://pypi.python.org/pypi/setuptools
[3]:https://pip.pypa.io/en/stable/installing/
[4]:http://supervisord.org/configuration.html

