---
layout : life
title: linux服务器监控psdash配置
category : Python学习
duoshuo: true
date : 2015-03-21
---

psdash是一款linux下的系统资源监控的web版本，和其相似的还有linux-dash.

<!-- more -->

**1.安装前的环境准备**

* 安装python,我用的环境是python 2. 7. 9

```sh
	sudo apt-get install python-pip
	sudo pip install python-dev
```

**2.安装psdash**

* 可以直接用pip进行安装

```sh
	sudo pip install psdash
```

* 或者用安装脚本

```sh
	sudo git clone  https://github.com/daodaoliang/psdash.git
	sudo cd  psdash
	sudo python setup.py install
```

* 也可以直接启用python

```sh
	sudo git clone  https://github.com/daodaoliang/psdash.git
	sudo cd  psdash
	sudo python ./psdash/run.py
```

**3. 启动测试**

* 直接测试

```sh
	sudo psdash -p 5000 -l '/var/log/*.log'
```

* 打开浏览器进行测试

```html
	http://127.0.0.1:5000
```

![截图](/res/img/blog/python学习/2015-03-21.png)
