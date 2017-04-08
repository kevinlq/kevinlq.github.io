---
layout : life
title: ubuntu下redis环境搭建
category : 环境搭建
duoshuo: true
date : 2015-03-24
---

<!-- more -->

******

## ***ubuntu下redis环境搭建***

### ***1. 下载安装***

```sh
	wget http://download.redis.io/redis-stable.tar.gz
```

然后

```sh
	sudo tar -zxvf redis-stable.tar.gz
	cd redis-stable
	sudo make
	sudo make test
	sudo make install
```

***通常在make test时发现以下错误：***

```sh
	You need tcl 8.5 or newer in order to run the Redis test
	make: *** [test] Error 1
```

则需要安装tcl 参考[这个网址](http://www.linuxfromscratch.org/blfs/view/cvs/general/tcl.html)

### ***2. 创建redis文件夹***

```sh
	sudo mkdir /etc/redis
	sudo mkdir /var/redis
```

### ***3. 复制修改启动脚本***

* 将启动脚本复制到/etc/init.d目录下

```sh
 	sudo cp utils/redis_init_script /etc/init.d/redis_6379
```

* 修改启动脚本

```sh
	sudo vim /etc/init.d/redis_6379
```

修改其中的REDIS_PORT为自己需要的端口，同时在脚本顶部加上：

```sh
	#chkconfig: 2345 80 90
	#description:auto_run
```
以确保服务支持chkconfig

### ***4. 创建redis的工作目录***

```sh
	sudo mkdir /var/redis/6379
```

### ***5. 修改配置文件***

* 将redis的配置文件拷贝到/etc/redis目录下，并修改或确认参数

```sh
	sudo cp redis.conf /etc/redis/6379.conf
	#设置daemonize为true
	#设置pidfile为/var/run/redis_6379.pid
	#设置loglevel
	#设置logfile为/var/log/redis_6379.log
	#设置dir为/var/redis/6379
	# requirepass foobared去掉注释，foobared改为自己的密码
```

### ***6. 设置启动脚本***

```sh
	/sbin/chkconfig --add redis_6379 && /sbin/chkconfig redis_6379 on
```

### ***7. 启动redis***

```sh
	sudo /etc/init.d/redis_6379 start 
```

### ***8. 开启防火墙的6379端口***

```
	这里就不在赘述了，前面的博客有写的～～
```






