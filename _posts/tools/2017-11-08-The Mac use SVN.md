---
layout: post
title: MAC下使用SVN
categories: [工具使用,SVN]
description: MAC下使用SVN
keywords: MAC, SVN
---

## 简介
MAC下面直接使用其自带的SVN就行，操作方法直接使用命令行.

## 使用

### 将文件checkout到本地

```C
svn checkout path(path是服务器上面的目录)

例如：svn checkout http://192.168.1.1:8080/svn/abc/version1 --username=zhangsan --passwrod=123
```
如果要更新到某个版本n,则后面需要添加版本号,比如:

```
svn checkout -r 10 http://192.168.1.1:8080/svn/abc/version1 

```

上面的 10代表版本号. **checkout** 还可以简写成 **co**


### 添加新的文件到版本中

```
svn add file
例如:svn add abc.cpp(只提阿尼甲abc.cpp文件)
svn add *.cpp(添加当前目录下的所有.cpp文件)
```

### 提交修改的文件
```
svn commit -m "add file"
例如:svn commit -m "add abc file" abc.cpp
```

### 更新到某个版本

```
svn update -r m path
例如：svn update（后面没有目录，默认将当前目录以及子目录所有文件进行更新）
svn update -r 200 abc.cpp(将版本库中abc.cpp还原到200)
SVN update abc.cpp(更新abc.cpp文件)
```

### 查看文件状态

```
svn status path
例如:svn status -v
```

### 查看日志

```
svn log path
例如:svn log abc.cpp(查看abc.cpp这个文件的修改历史)
```

### 比较差异

```
svn diff path
例如：svn diff abc.cpp

svn diff -r m:n path(对版本m和n进行比较)
例如：svn diff -r 200:201 abc.cpp
```

```
svn diff

上述命令执行后，会比较当前版本和当前缓冲的差异.
```

### 筛选出修改过的文件进行提交

命令:
```
svn st | grep "M" | cut -c 8->modified.txt
```
以上命令会将修改过的文件路径以及名存入文件中，打开文件确认那些需要提交，那些不需要，删除掉不需要的保存。

提交:

```
svn ci -m "add update..." --targets modified.txt
```

### 帮助 help
```
svn help
```

## 参考文章

[http://www.cnblogs.com/luckythan/p/4478706.html](http://www.cnblogs.com/luckythan/p/4478706.html)
