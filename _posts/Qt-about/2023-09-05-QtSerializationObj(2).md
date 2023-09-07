---
layout: post
title: Qt 对象序列化/反序列化
categories: [Qt]
description: 序列化对象
keywords: Qt, C++
---

> 上一篇文章介绍了自动进行序列化和反序列化的方法，其中也指出了其中存在的问题以及不足，今天这篇文章来详细说下如何处理


## 背景

目前使用自动生成属性的方法支持的数据类型有限，并不是所有的数据类型有支持，不支持的类型生成的键值对的值会是 `null`，肯定不是我们想要的，因此需要对这些类型进行自定义扩展。

目前不支持的类型主要有以下几种场景：

*   QRect
*   QSize
*   QPoint
*   QList\<T>，QVector\<T>，比如： QList\<int>，QList\<float>，QList\<double>……
*   QPolygon, QPolygonF
*   QLine，QLineF
*   QMap, QSet,QHash……
*   std 标准库数据类型
*   其它自定义类型

### 原因

为什么上述类型不支持呢？有两个原因：

第一，上述这些类型无法反向解析，无法进行有效的区分，比如你想把一个`QSize` 保存成什么样子？ 

- 数组形式：[10,10]
- 字符串："QSize(10,10)"
 
不管哪种方式，都不是通用的，数组格式在反序列化时无法还原，除非手动进行，字符串方式有冗余字段无法满足三方使用。

第二，其实不怪 `Qt`，`JSON` 的键值就支持这些类型,[侯捷] 曾说过「源码面前了无秘密」,我们再顺便看下`Qt`源码。

某个字段的值是这样获取的，返回的是一个 QVariant 类型，这个类型本身可以支持多种数据类型

```C++
QVariant v = object->property(proName);
```

将获取的键值插入到 `QJsonObject` 当中:
```C++
jsObj.insert(proName, QJsonValue::fromVariant(v));
```
问题就出在这里，`JSON` 对象的值需要一个 `QJsonValue` 类型，但是 `QJsonValue` 仅仅支持常见的基本数据类型
```C++
    QJsonValue(Type = Null);
    QJsonValue(bool b);
    QJsonValue(double n);
    QJsonValue(int n);
    QJsonValue(qint64 v);
    QJsonValue(const QString &s);
    QJsonValue(QLatin1String s);
    QJsonValue(const QJsonArray &a);
    QJsonValue(const QJsonObject &o);
```

基于上述两个原因，我们能够做的只能是把其它类型转换成标准`JSON` 支持的类型。

## 方案

JSON作为通用的数据格式，一般普遍做法需要针对特殊类型字段单独处理，再反序列化时也需要单独处理，下面以`QPoint、QSize` 两种类型为例详细展开说下:

```C++
    Q_PROPERTY(QSize testSize READ testSize WRITE setTestSize)
    Q_PROPERTY(QPoint testPoint READ testPoint WRITE setTestPoint)
```

在进行序列化时，判断该属性类型，然后分别进行处理：

```
    switch (propertyType)
    {
    case QMetaType::QSize:      return serializeSize(value.toSize());
    case QMetaType::QSizeF:     return serializeSize(value.toSizeF());
    default:                    throw  KException{ QByteArray("Invalid type id ") +
                         QByteArray(QMetaType::typeName(propertyType))};
    }
```

QSize和QSizeF 是类似的，因此需要写一个模板来统一处理：
```C++
template <class Size>
static inline QVariant serializeSize(const Size &size)
{
    return QVariant(QJsonArray{size.width(), size.height()});
}
```
**扩展**

一般我们能想到的是分别实现对`QSize`和`QSizeF` 进行处理，这个时候你会写两个函数来处理，进阶后你会写一个模板来处理，那么再次进阶下，怎么处理呢？

来看一个更高级的用法(语法糖，C++17才有的)
```C++
QJsonValue serializeSize(const std::variant<QSize, QSizeF> &size) const
{
	return
		std::visit([](const auto &s) -> QJsonArray {
			return {s.width(), s.height()};
		}, size);
}
```
> 上述代码可以参考开源项目: https://github.com/Skycoder42/QtJsonSerializer

## 总结

上述提供了一种方案和思路，按照这个思路继续扩展其它数据类型即可，如果感兴趣可以直接看这个开源项目：https://github.com/Skycoder42/QtJsonSerializer， 不过需要 `Qt5.12` 及以上版本才支持哦

如果想自己动手实现，顺便深入学习下 `Qt` 元对象系统，那么可以一起参与进来，从零实现一个简易版本的序列化库: https://github.com/kevinlq/KSerialize.


授人以鱼不如授人以渔， 方案和思路有了，关键还是要多动手写起来,如果有问题随时留言交流。


******

    作者:鹅卵石
    时间: 2023年9月5日23:41:31
    版本:V 0.0.1
    邮箱:kevinlq@163.com
    版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
    希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->

---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**