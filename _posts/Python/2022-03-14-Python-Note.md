---
layout: post
title: Python 学习笔记
categories: [Python]
description: Python 常用使用笔记总结
keywords: Python
---


## 简介

[参考地址](https://www.liaoxuefeng.com/wiki/1016959663602400/1017063826246112)


## 软件安装

开始安装

![download](/res/img/blog/Python/download.png)

Windows 系统

![install](/res/img/blog/Python/install.png)

注意上图，将Python添加到系统环境变量中

点击 Install Now 开始安装

打开命令行，输入Python

![install_test](/res/img/blog/Python/install_test.png)

出现上图显示内容，则表示安装成功了

## 基本知识

### 注释

```C++
# 这是一个注释
print("Hello, World!")
```

单引号

```C++
#!/usr/bin/python3 
'''
这是多行注释，用三个单引号
这是多行注释，用三个单引号 
这是多行注释，用三个单引号
'''
print("Hello, World!")
```

双引号

```C++
#!/usr/bin/python3 
"""
这是多行注释，用三个双引号
这是多行注释，用三个双引号 
这是多行注释，用三个双引号
"""
print("Hello, World!")
```

### 循环

循环数组

```C++
#第一种，for in的语法
for item in items:
　　print (item)
#第二种是下标访问
for index in range(len(items)):
　　print (index,items[index])

#第三种是enumerate,会生成索引和元素
for index,item in enumerate(items):
　　print (index, item)
```


## 一些框架

### Django

[官方介绍](https://docs.djangoproject.com/zh-hans/3.1/)

安装
```C++
pip install Django==3.1.5
```


安装完成后测试是否成功
```Python
>>> import django
>>> print(django.get_version())
3.1

或者
python -m django --version
```

创建应用

```
python manage.py startapp demo1
```

启动测试服务
```
python manage.py runserver
```

## 第三方库安装

国内镜像

- [阿里云 ](http://mirrors.aliyun.com/pypi/simple/)
- [中国科技大学](https://pypi.mirrors.ustc.edu.cn/simple/)
- [豆瓣(douban)](http://pypi.douban.com/simple/)
- [清华大学](https://pypi.tuna.tsinghua.edu.cn/simple/)
- [中国科学技术大学](http://pypi.mirrors.ustc.edu.cn/simple/)


### requests

```
pip install requests
```

正常情况下回安装成功，但是也有意外，比如我在windows10系统上第一次安装时超时

安装失败

![requests_install](/res/img/blog/Python/requests_install_error.png)

说明你采用了默认的pypi源(国外的pypi源)，这个很容易出现这种连接超时的问题，所以应当采用国内的镜像源。

在你需要安装的xx后面添加-i + pypi源：
```
pip install requests -i http://pypi.douban.com/simple
```

如果还出现下面的情况
```
pypi.douban.com is not a trusted or secure host and is being ignored...
```

那么命令就变成这样
```
pip install requests  -i http://pypi.douban.com/simple --trusted-host pypi.douban.com
```

惊喜时刻，安装成功了，如下图所示
![requests_install_ok](/res/img/blog/Python/requests_install_ok.png)

### bs4
```
pip install bs4 -i http://pypi.douban.com/simple --trusted-host pypi.douban.com
```

### lxml
```
pip install lxml -i http://pypi.douban.com/simple --trusted-host pypi.douban.com
```

### pandas
```
pip install --upgrade pandas -i https://pypi.douban.com/simple
```

### ORM框架

#### SQLObject

#### Storm

#### peewee

```
pip install peewee


如果安装失败，则可以添加源
pip install peewee -i http://pypi.douban.com/simple --trusted-host pypi.douban.com
```
![peewee_install_ok](/res/img/blog/Python/peewee_install_ok.png)

#### SQLAlchemy


### mkdocs
```
pip install mkdocs

pip install mkdocs -i https://pypi.mirrors.ustc.edu.cn/simple/
```

确认是否安装正确
```
mkdocs --version
```

创建wiki
```
mkdocs new my-wiki
cd my-wiki
```

启动服务
```
mkdocs serve
```
打开浏览器输入127.0.0.1:8000访问wiki

安装主题
```
pip install mkdocs-material

pip install mkdocs-material -i https://pypi.mirrors.ustc.edu.cn/simple/

pip install mkdocs-rtd-dropdown -i https://pypi.mirrors.ustc.edu.cn/simple/
```

在配置文件中进行配置
```
theme:
  name: 'material'
```

修改端口号
[参考文档](https://squidfunk.github.io/mkdocs-material/getting-started/)


### scrapy

```
pip install scrapy -i https://pypi.mirrors.ustc.edu.cn/simple/
```

## 常见错误

### 更新 pip

```
you should consider upgrading via the python -m pip install --upgrade pip command.....
``` 

解决方案
核心命令
```
python -m pip install -U pip
```

上述命令可能会失败，因为超时失败，可以指定国内某个镜像源
```
python -m pip install -i http://pypi.douban.com/simple --trusted-host pypi.douban.com -U pip
```

![pip_install_error](/res/img/blog/Python/pip_install_error.png)

进入到提示报错的目录中，删除掉 c:\users\devstone\appdata\local\programs\python\python38\lib\site-packages (20.1)
删除 pip-20.1.dist-info 文件夹，然后重新执行更新命令即可

![pip_install_ok](/res/img/blog/Python/pip_install_ok.png)





## 参考文章

- [Python 官方](https://www.python.org/)

******

    作者:鹅卵石
    时间: 2021年3月20
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