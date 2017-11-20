---
layout : post
title: C++中this与*this的区别
category : C++学习
wangyiyun: true
date : 2015-11-01
---

******

    作者:鹅卵石
    时间:2015年11月01日23:04:16
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

# C++中this与*this的区别

在一个类里每个成员函数都有一个this指针

this指针指向调用对象，如果方法需要引用整个调用对象可以使用*this

this是对象地址，而不是对象本身

*this则是对象本身，可以将*this作为调用对象的别名

```
stock::stock(const std::string &name)
{
    this->m_company = name;
    (*this).m_company = "lh";
}

```

##  什么时候需要this指针

1. 在类的非静态成员函数中返回对象的本身时候
这种情况下可以直接返回
```
return *this;
```

2. 传入的形参与成员变量名相同
比如
```
this->name = name;
```
注意不能写成:
```
name = name;
```

## this指针特点

1. 只能在成员函数中使用
2. 在成员函数开始前构造，在成员函数结束后清除

## 网络上的一个经典回答

>当你进入一个房子后   
你可以看见桌子、椅子、地板等   
但是你是看不大房子的全貌的   
对于一个类的实例来说   
你可以看到它的成员函数、成员变量    
但是实例本身呢？
你是看不到的   
this是一个指针，它时时刻刻指向你这个实例本身


---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)





