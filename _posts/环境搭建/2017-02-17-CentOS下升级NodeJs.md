---
layout : life
title : CentOS6.5下升级NodeJs
category : 环境搭建
duoshuo : true
date : 2017-02-17
---


******

	作者: daodaoliang
    版本: V 1.0.1
    邮箱：daodaoliang@yeah.net

<!-- more -->

  `NodeJs` 是什么就不在赘述，可一直接参考[英文官网][1]，或者[中文官网][2]，在项目中进行应用时，有时候需要用到最新版本的特性，但是CentOS系统为了稳定性，默认的源都是在该版本的操作系统发布时或者更新时的最稳定版本，所以仅仅是采用`yum update` 的操作是不能进行对应升级的。
  所以下面采用两种方式来进行在线升级。
  
### 0x01 用 `n` 模块来进行 `nodejs` 升级

  在 `NodeJs` 中有一个模块叫做 `n` 专门用来管理 `NodeJs` 的版本。
  
  * 首先下载安装模块 `n` 
  
  ```
    npm install -g n
  ```
  
  * 升级`NodeJs`到最新版本
  
  ```
    n stable
  ```
  
  * 升级`NodeJs`到指定版本
  
  ```
    n 0.10.23
  ```

### 0x04 用编译好的二进制文件进行安装更新

  在官网下载相关系统平台下面编译好的二进制包。
  
  ```
    cd node-v0.10.28-linux-x64/bin
    ls
    ./node -v
  ```
  
  上面的二进制文件就是编译好的新版本的`node`文件，不过并没有添加到环境变量里面，所以可以通过下面这种方式进行设置为全局变量
  
  ```
   ln -s /home/kun/mysofltware/node-v0.10.28-linux-x64/bin/node /usr/local/bin/node
   ln -s /home/kun/mysofltware/node-v0.10.28-linux-x64/bin/npm /usr/local/bin/npm
  ```
  
### 0x03 用源码的方式进行编译安装

一般用方式一就可以完成升级安装，但是有时候会出现一些莫名其妙的错误，比如说是`segment fault`之类的错误，这个时候我只能用源码的方式进行编译安装。

首先`NodeJs`使用`C++`语言编写的，所以在编译源码时需要用到gcc-g++编译器，目前最新的`NodeJs`代码用到了`c11`的一些特性，所以对编译器的版本要求在4.8+所以首先要对编译器进行安装和升级。

 * 安装并升级编译器到4.8+
 
 ```
   yum install gcc-c++ openssl-devel
   wget http://people.centos.org/tru/devtools-2/devtools-2.repo
   mv devtools-2.repo /etc/yum.repos.d
   yum install devtoolset-2-gcc devtoolset-2-binutils devtoolset-2-gcc-c++
 ```
 
 三个安装包被安装在`/opt/rh/devtoolset-2/root/`中
 
 更新软连接:
 
 ```
  mv /usr/bin/gcc /usr/bin/gcc-4.4.7 
  mv /usr/bin/g++ /usr/bin/g++-4.4.7 
  mv /usr/bin/c++ /usr/bin/c++-4.4.7 
  ln -s /opt/rh/devtoolset-2/root/usr/bin/gcc /usr/bin/gcc 
  ln -s /opt/rh/devtoolset-2/root/usr/bin/c++ /usr/bin/c++ 
  ln -s /opt/rh/devtoolset-2/root/usr/bin/g++ /usr/bin/g++ 
  gcc --version
 ```
  
  * 检查核对`python`版本
  
  `nodejs`需要python版本在2.6版本以上，我们需要检查版本是否对应符合，如果不符合那就进行一下升级；
  
  * 编译安装最新版本的`nodejs`
  
  ```
   cd /usr/local/src
   wget http://nodejs.org/dist/node-latest.tar.gz
   tar -zxvf node-latest.tar.gz
   cd node-v0.12.7
   ./configure
   make && make install
  ```
  
 不管是用哪种方式进行升级都需要很长的时间，休息一会等着就好了，升级完成后查看一下版本信息；
 
 ```
	node -v
 ```
 
 **PS:** 在升级的过程中把`node` 运行的相关程序要关闭，不然会在编译时由于文件占用的关系产生错误.
   
  

[1]:https://nodejs.org/en/
[2]:http://nodejs.cn/