---
layout : post
title: C++中explicit用法总结
category : C++学习
wangyiyun: true
date : 2017-05-06
---

******

    作者:鹅卵石
    时间:2017年5月6日20:39:40
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

## C++中explicit用法总结

在编写Qt相关程序时经常能看见构造函数前面添加explicit，今天想起来就总结下，做做笔记

### explicit 作用

在C++中，explicit关键字用来修饰类的构造函数，若类的构造函数前面被explicit修饰，则该类不能发生隐式类型转换，只能以显示的方式进行类型转换。这句话可能有点抽象，什么叫隐式和显示转换？下面举个栗子

people.h
```
#ifndef PEOPLE_H
#define PEOPLE_H


class People
{
public:
    People(int age);

    void printAge();

private:
    int m_age;
};

#endif // PEOPLE_H
```

people.cpp
```
#include "people.h"

#include <QDebug>

People::People(int age):m_age(age)
{
}

void People::printAge()
{
    qDebug()<<"age:"<<m_age;
}

```

main.cpp
```
#include <QApplication>

#include "people.h"

void function()
{
    People p1(10);
    p1.printAge ();

    People p2 = 20;
    p2.printAge ();
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    //QApplication a(argc, argv);

    function ();

    return 0;
}
```

----

在这一段C++程序中我们定义了一个类people，包含了一个构造函数，该构造函数只包含了一个整形参数，然后再初始化列表中对其进行了初始化

在function函数中使用两种方式创建了10岁和20岁的人，第一种方式属于常见的类变量声明方式；第二种方式属于特殊方式，没有经过任何显示的转换，就直接将整形变量赋值给类变量p2

**C/C++是一种强类型语言，不同的数据类型是不能随意进行转换的，必须进行显示的强制类型转换**

编译上述代码，运行后可以直接输出

```
age: 10
age: 20

```

现在添加explicit修饰类people，只需在类前面添加修饰符即可

```
#ifndef PEOPLE_H
#define PEOPLE_H


class People
{
public:
    explicit People(int age);

    void printAge();

private:
    int m_age;
};

#endif // PEOPLE_H
```


编译时则出现了错误提示

![explicit编译错误](/res/img/blog/C++学习/explicit_error.png)

这样以来可以避免隐式类型转换

### explicit 使用事项

* explicit关键字只能用类的内部构造函数的声明上；
* explicit关键字用于单个参数的构造函数；
* 在C++中，explicit用来修饰类的构造函数，被修饰的构造函数的类不能发生相应的隐式类型转换；



---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)





