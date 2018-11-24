---
layout: post
title: 设计模式之简单工厂模式
categories: 设计模式
description: 简单工厂模式
keywords: 设计模式
---

简单工厂模式


## 介绍

当你需要什么时，只需要传入一个正确的参数，就可以获取所需要创建的对象，无须知道创建的细节(创建过程).

>比如： 我们去餐厅吃饭，只需要报下菜名，厨房做好饭后会送到我们面前，而我们无须知道饭时怎么做的.


## 模式定义

**简单工厂模式** (simple factory pattern) 又称为静态工厂方法模式，属于类创建型模式。可以根据传入的不同参数返回不同类的实例.


## 栗子

实现一个简单的计算器，实现2个数的运算(加，减，乘，除)

### 计算基类

```C++
class OperatorBase
{
public:
    OperatorBase();
    virtual ~OperatorBase() = 0;

    void setNumberA(double x);
    double getNumberA();

    void setNumberB(double y);
    double getNumberB();

    double virtual getResult() = 0;

private:
    DataPrivate *m_dPtr;
};
```

- 加法类实现

```C++
class OperatorAdd : public OperatorBase
{
public:
    OperatorAdd();
    ~OperatorAdd();

    double virtual getResult();
};

double OperatorAdd::getResult()
{
    double result;

    result = getNumberA() + getNumberB();

    return result;
}
```

### 工厂方法类实现

```C++
class OperatorFactory
{
public:
    OperatorFactory();
    ~OperatorFactory();

    static OperatorBase * createOperate(OPERATOR op);
};

OperatorBase *OperatorFactory::createOperate(OPERATOR op)
{
    OperatorBase *oper = nullptr;

    switch (op)
    {
    case OP_ADD:
        oper = new OperatorAdd;
        break;
    case OP_SUB:
        oper = new OperatorSub;
        break;
    case OP_MUL:
        oper = new OperatorMul;
        break;
    case OP_DIV:
        oper = new OperatorDiv;
        break;
    default:
        oper = nullptr;
    }
    return oper;
}
```

### 使用

main.cpp

```C++
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int status = 0;
    status = AssertTest( new TestSimpleFactory());

    return status;
}
```

这里以及以后都使用Qt自带的单元测试框架 `QTestLib`, 使用起来还是很方便的. 具体的测试代码如下:

```C++
void TestSimpleFactory::testAddOperator()
{
    OperatorBase *pOperAdd = OperatorFactory::createOperate(OP_ADD);
    QVERIFY2(pOperAdd != Q_NULLPTR, "oper is null");
    pOperAdd->setNumberA(12);
    pOperAdd->setNumberB(4);

    double result = pOperAdd->getResult();
    qDebug()<<"and result:"<<result;

    delete pOperAdd;
}

void TestSimpleFactory::testSubOperator()
{
    OperatorBase *pOperSub = OperatorFactory::createOperate(OP_SUB);
    QVERIFY2(pOperSub != Q_NULLPTR, "oper is null");
    pOperSub->setNumberA(12);
    pOperSub->setNumberB(4);

    double result = pOperSub->getResult();
    qDebug()<<"sub result:"<<result;

    delete pOperSub;
}

void TestSimpleFactory::testMulOperator()
{
    OperatorBase *pOperMul = OperatorFactory::createOperate(OP_MUL);
    QVERIFY2(pOperMul != Q_NULLPTR, "oper is null");
    pOperMul->setNumberA(12);
    pOperMul->setNumberB(4);

    double result = pOperMul->getResult();
    qDebug()<<"mul result:"<<result;

    delete pOperMul;
}

void TestSimpleFactory::testDivOperator()
{
    OperatorBase *pOperDiv = OperatorFactory::createOperate(OP_DIV);
    QVERIFY2(pOperDiv != Q_NULLPTR, "oper is null");
    pOperDiv->setNumberA(12);
    pOperDiv->setNumberB(4);

    double result = pOperDiv->getResult();
    qDebug()<<"div result:"<<result;

    delete pOperDiv;
}
```

输出结果:

```
********* Start testing of TestSimpleFactory *********
Config: Using QtTest library 5.10.0, Qt 5.10.0 (i386-little_endian-ilp32 shared (dynamic) release build; by GCC 5.3.0)
PASS   : TestSimpleFactory::initTestCase()
QDEBUG : TestSimpleFactory::testAddOperator() and result: 16
PASS   : TestSimpleFactory::testAddOperator()
QDEBUG : TestSimpleFactory::testSubOperator() sub result: 8
PASS   : TestSimpleFactory::testSubOperator()
QDEBUG : TestSimpleFactory::testMulOperator() mul result: 48
PASS   : TestSimpleFactory::testMulOperator()
QDEBUG : TestSimpleFactory::testDivOperator() div result: 3
PASS   : TestSimpleFactory::testDivOperator()
PASS   : TestSimpleFactory::cleanupTestCase()
Totals: 6 passed, 0 failed, 0 skipped, 0 blacklisted, 12ms
********* Finished testing of TestSimpleFactory *********
```


## 优缺点

### 优点

- 对象的创建进行了延迟，避免了直接创建对象的责任；
- 无须知道所创建的具体产品类的类名，只需要知道创建参数即可；
- 可以适当的通过引入配置文件，在不修改代码的情况下，更换和更新具体的产品类，在一定程度上提高了系统的灵活性；


### 缺点

任何优点有时候也会是它的缺点！

>由于所有的产品类创建逻辑都集中在了工厂类中，一旦工厂类不能工作，整个系统都要受到影响.

- 系统扩展困难，一旦需要添加新的产品，就必须修改工厂中产品创建的逻辑(违背了开放-关闭原则)；
- 当产品类过多时，可能造成工厂逻辑过于复杂，不利于系统的维护和扩展；
- 工厂模式使用了静态工厂方法，造成了工厂角色无法形成基于继承的等级结构;




## 参考文章

- [Bobby0322](http://www.cnblogs.com/Bobby0322/p/4178412.html/)

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
