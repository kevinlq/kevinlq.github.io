---
layout: post
title: 设计模式之单例模式
categories: 设计模式
description: 单例模式
keywords: 设计模式
---

单例模式，顾名思义，就是当前类只有一个实例,不论被项目中那个类或者对象调用.

## 想一想

- 单例类如何创建?
- 单例类如何销毁？需要销毁么？
- 如何做到线程安全？
- 如何测试?
- 使用场景


## 分类
单例模式目前比较流行的有两种: **饿汉**与**懒汉**

- 饿汉
饿了肯定会饥不择食，想办法吃点，所以在单例类定义的时候就进行了初始化

- 懒汉
因为比较懒，所以不到完不得以不会初始化。在第一次使用到单例类时才初始化。

### 选择和特点

- 如果要进行线程同步操作，访问量比较大，采用**饿汉**方式，这样可以实现更好的性能 (以空间换时间);

- 访问量较小时，采用**懒汉**实现 (以时间换空间)

## 实现方式

构造函数私有，添加静态访问接口;

### 实现方式一 (普通方式)

SingletonDemo.h

```C++
class SingletonDemo
{
private:
    SingletonDemo();
    ~SingletonDemo();

public:
    static SingletonDemo * getInstance();
    static void destroyInstance();

    int getAdd( int nValueA, int nValueB);

private:
    static SingletonDemo *m_pInstance;
};
```
SingletonDemo.cpp

```C+++
#include "SingletonDemo.h"

SingletonDemo* SingletonDemo::m_pInstance = nullptr;

SingletonDemo::SingletonDemo()
{
}

SingletonDemo::~SingletonDemo()
{
}

SingletonDemo *SingletonDemo::getInstance()
{
    if ( m_pInstance == nullptr)
    {
        m_pInstance = new SingletonDemo;
    }

    return m_pInstance;
}

void SingletonDemo::destroyInstance()
{
    if ( m_pInstance != nullptr)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

int SingletonDemo::getAdd(int nValueA, int nValueB)
{
    return (nValueA + nValueB);
}
```

main.cpp

```C++
#include <QCoreApplication>

#include <QDebug>

#include "SingletonDemo.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int nSum = SingletonDemo::getInstance ()->getAdd (1,2);
    qDebug()<<"nSum:"<<nSum;

    int nRet = a.exec();

    SingletonDemo::getInstance ()->destroyInstance ();

    return nRet;
}
```

上述实现方式很简单，也很容易看懂。但是不是线程安全的！

>如果此时有多个线程同时首次调用方法`SingletonDemo::getInstance ()`,那么，将有可能同时检测到指针`m_pInstance` 为 `nullptr`,这样，这两个线程将会同时构造2个实例！

### 实现方式二 (加锁实现方式)

上述实现方式不是线程安全的，可以通过加锁优化.

核心实现方式

```C++
SingletonDemo *SingletonDemo::getInstance()
{
    if ( m_pInstance == nullptr)
    {
        lock();
        if ( m_pInstance == nullptr)
        {
            m_pInstance = new SingletonDemo;
        }
        UnLock();
    }

    return m_pInstance;
}

...
destroy单例类
...
```

可以看出，在获取单例时，如果为空则加锁，在判断是否为空，如果还为空再进行初始化操作。这样实现方式为什么是线程安全的？

可以这样想想，当第一个线程获取单例时，发现`m_pInstance`为空，那么直接加锁，再去判断是否为空，当此时另一个线程也来访问时，发现此时`m_pInstance`为空，想着要初始化单例时，发现资源占用(加锁了)，所以只能等待，这样才是线程安全的。

>劣势：这样实现方式在平时一般项目中可以，但是在进行大数据操作的时候就有性能瓶颈！

### 实现方式三 (自动销毁实现方式)

```C++
#ifndef SINGLETONDEMO_H
#define SINGLETONDEMO_H


class SingletonDemo
{
private:
    SingletonDemo();
    ~SingletonDemo();

public:
    static SingletonDemo * getInstance();

    int getAdd( int nValueA, int nValueB);

private:
    static SingletonDemo *m_pInstance;

    class GC
    {
    public:
        ~GC()
        {
            if ( m_pInstance != nullptr)
            {
                delete m_pInstance;
                m_pInstance = nullptr;
            }
        }
    };
    static GC m_gc;
};

#endif // SINGLETONDEMO_H
```

```C++
#include "SingletonDemo.h"

SingletonDemo* SingletonDemo::m_pInstance = new SingletonDemo;
SingletonDemo ::GC SingletonDemo::m_gc;

SingletonDemo::SingletonDemo()
{
}

SingletonDemo::~SingletonDemo()
{
}

SingletonDemo *SingletonDemo::getInstance()
{
    return m_pInstance;
}

int SingletonDemo::getAdd(int nValueA, int nValueB)
{
    return (nValueA + nValueB);
}
```

main.cpp

```C++
#include <QCoreApplication>

#include <QDebug>

#include "SingletonDemo.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int nSum = SingletonDemo::getInstance ()->getAdd (1,2);
    qDebug()<<"nSum:"<<nSum;

    int nRet = a.exec();
    return nRet;
}
```
程序在结束时，系统会自动调用 `SingletonDemo` 静态成员函数 `GC` 的析构函数，对单例进行释放。


### 实现方式四 (优化方式)

```C++
#ifndef SINGLETONDEMO_H
#define SINGLETONDEMO_H

class SingletonDemo
{
private:
    SingletonDemo();
    ~SingletonDemo();

public:
    static SingletonDemo * getInstance();

    int getAdd( int nValueA, int nValueB);
};

#endif // SINGLETONDEMO_H
```

```C++
#include "SingletonDemo.h"

SingletonDemo::SingletonDemo()
{
}

SingletonDemo::~SingletonDemo()
{
}

SingletonDemo *SingletonDemo::getInstance()
{
    static SingletonDemo instance;
    return &instance;
}

int SingletonDemo::getAdd(int nValueA, int nValueB)
{
    return (nValueA + nValueB);
}
```

main.cpp

```C++
#include <QCoreApplication>

#include <QDebug>

#include "SingletonDemo.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int nSum = SingletonDemo::getInstance ()->getAdd (1,2);
    qDebug()<<"nSum:"<<nSum;

    int nRet = a.exec();
    return nRet;
}
```

上述实现方式很好的避免了很多的问题，比如单例类的销毁问题，由于单例并没有进行 `new` 出，所以没有看到用 `delete` 进行释放。

静态变量存储在静态存储区，在系统退出时会自动释放。

>上述实现方式也是比较推荐的方式，当然在实际项目中还要根据需求进行取舍.


### 单例模板

实现一个单例类很快的，如果项目中使用的单例类多，那么也是挺繁琐的，可以通过写一个单例模板来解决。

```C++
template<typename T>
class Singleton
{
public:
    static T* instance(){
        static T instance;
        return &instance;
    }

private:
    Singleton();
    ~Singleton();
    Singleton(const Singleton &);
    Singleton & operator = (const Singleton &);
};
```

#### use

```C++
class A{
    public:
    void foo(){//DO FOO}
}

int main()
{
    A *a = Singleton<A>::instance();
    a->foo();
    return 0;
}
```

上面这个也存在线程安全问题，可以这样进行修改下:

### 单例模板2

```C++
#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <QScopedPointer>

/**
 * 使用方法:
 * 1. 定义类为单例:
 *     class A {
 *         SINGLETON(A) // Here
 *     public:
 *
 * 2. 获取单例类的对象:
 *     Singleton<A>::getInstance();
 *     A &pool = Singleton<A>::getInstance();
 */

namespace Pattern
{

template <typename T>
class Singleton
{
public:
    static T& getInstance();

    Singleton(const Singleton &other);
    Singleton<T>& operator=(const Singleton &other);

private:
    static QMutex mutex;
    static QScopedPointer<T> instance;
};

/*-----------------------------------------------------------------------------|
 |                          Singleton implementation                           |
 |----------------------------------------------------------------------------*/
template <typename T> QMutex Singleton<T>::mutex;
template <typename T> QScopedPointer<T> Singleton<T>::instance;

template <typename T>
T& Singleton<T>::getInstance()
{
    if (instance.isNull())
    {
        mutex.lock();
        if (instance.isNull())
        {
            instance.reset(new T());
        }
        mutex.unlock();
    }

    return *instance.data();
}

/*-----------------------------------------------------------------------------|
 |                               Singleton Macro                               |
 |----------------------------------------------------------------------------*/
#define SINGLETON(Class)                        \
    private:                                    \
    Class();                                    \
    ~Class();                                   \
    Class(const Class &other);                  \
    Class& operator=(const Class &other);       \
    friend class Singleton<Class>;              \
    friend struct QScopedPointerDeleter<Class>;

}
#endif // SINGLETON_H

```

#### 使用：

```C++
class MyClass
{
    SINGLETON(MyClass)
    public:
};
MyClass::MyClass(){}
MyClass::~MyClass (){}

typedef Singleton<MyClass> SSClass;

int main(int argc, char *argv[])
{
    SSClass::getInstance ();
}
```



## 参考文章

- [伯乐在线](http://blog.jobbole.com/108579/)

******

    作者:鹅卵石
    时间:2018年5月17日21:07:22
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="/res/img/myCode.png" width="80%" height="80%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---
