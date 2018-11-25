---
layout: post
title: 设计模式之工厂方法模式
categories: 设计模式
description: 工厂方法模式
keywords: 设计模式
---

工厂方法模式


## 模式定义

**工厂方法模式** (factory methor pattern) ,又称为多态性工厂模式. 在此模式中，工厂类不在负责创建所有的产品，而将具体的创建工作交给子类去实现。

>工厂方法模式是简单工厂模式的衍生，解决了许多简单工厂模式的问题

- 实现 `开-闭` 原则；
- 可扩展，适合更加负责的软件结构以及场合；


## 解决问题

在 `简单工厂模式` 中，如果将我们需要新添加一个运算符，就需要修改工厂方法创建逻辑了，这不是很合理，所以就有了工厂方法模式。


## 栗子

还是以之前的计算器例子进行扩展.

<center>
<img src="/res/img/blog/Design_Pattern/factory_methor.png" width="80%" height="80%" />
</center>


### 关键部分代码

- 抽象工厂类

```
class FactoryBase
{
public:
    FactoryBase();
    virtual ~FactoryBase();

    virtual OperatorBase *CreateOperate() = 0;
};
```

- 加法工厂类

```
class FactoryAdd : public FactoryBase
{
public:
    FactoryAdd();
    virtual ~FactoryAdd();

    virtual OperatorBase *CreateOperate();
};


OperatorBase *FactoryAdd::CreateOperate()
{
    return new OperatorAdd();
}
```

其他运算符操作类和之前的简单工厂模式中类似

- 测试使用

```
void TestFactoryMethor::testAddOperator()
{
    m_pFactory = new FactoryAdd();
    QVERIFY2(m_pFactory != NULL, "pFactory is null");

    m_pOperate = m_pFactory->CreateOperate ();
    QVERIFY2(m_pOperate != NULL, "pOpera is null");

    m_pOperate->setNumberA (6);
    m_pOperate->setNumberB (3);

    double dResult = m_pOperate->getResult ();

    qDebug()<<"add result:"<<dResult;

    QVERIFY2( dResult == 9, "result is error!");
}
```

- 输出结果

```
********* Start testing of TestFactoryMethor *********
Config: Using QtTest library 5.10.0, Qt 5.10.0 (i386-little_endian-ilp32 shared (dynamic) release build; by GCC 5.3.0)
PASS   : TestFactoryMethor::initTestCase()
QDEBUG : TestFactoryMethor::testAddOperator() add result: 9
PASS   : TestFactoryMethor::testAddOperator()
QDEBUG : TestFactoryMethor::testSubOperator() sub result: 3
PASS   : TestFactoryMethor::testSubOperator()
QDEBUG : TestFactoryMethor::testMulOperator() mul result: 18
PASS   : TestFactoryMethor::testMulOperator()
QDEBUG : TestFactoryMethor::testDivOperator() div result: 2
PASS   : TestFactoryMethor::testDivOperator()
PASS   : TestFactoryMethor::cleanupTestCase()
Totals: 6 passed, 0 failed, 0 skipped, 0 blacklisted, 3ms
********* Finished testing of TestFactoryMethor *********
```



## 代码下载

- [GitHub](https://github.com/kevinlq/design_Pattern)

## 优缺点


### 优点
- 工厂方法增加了灵活性，可以使用基类的缺省实现也可以重新实现;
- 屏蔽了对应的产品类，调用者不需要关系，值需要关心产品的接口即可;
- 多态性，客户代码可以做到与特定应用无关，适用于任何实体类;


### 缺点
- 需要创建对应产品的工厂；............有待商榷



## 参考文章

- [请叫我头头哥](https://www.cnblogs.com/toutou/p/4899388.html)

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
<img src="/res/img/myCode.png" width="50%" height="50%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---
