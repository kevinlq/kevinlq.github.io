---
layout: post
title: QxOrm build for all platform
categories: [工具]
description: 编译QxOrm
keywords: Qt，QxOrm
---

QxOrm 是一个基于QT开发的数据库方面的ORM库,功能很强大.是QT C++数据开发方面的好工具。

官网 [http://www.qxorm.com/](http://www.qxorm.com/) ，目前进不去了。


## 错误

```
E:\****\QxOrm\QxOrm\include\QxSerialize\QDataStream\QxSerializeQDataStream_QFlags.h:58: error: redefinition of 'template<class T> QDataStream& operator>>(QDataStream&, QFlags<T>&)'
 QDataStream & operator>> (QDataStream & stream, QFlags<T> & t)
 
 E:\***\QxOrm\QxOrm\include\QxSerialize\QDataStream\QxSerializeQDataStream_QFlags.h:50: candidate: QDataStream& operator<<(QDataStream&, const QFlags<T>&) [with T = QSql::ParamTypeFlag]
 QDataStream & operator<< (QDataStream & stream, const QFlags<T> & t)
```

很明显，运算符重载方法重复定义了.

查找该方法定义的地方，发现是Qt自带的 `QDataStream` 类中已经有了。在之前Qt5.7中没有发现这个问题，
看来是Qt5.9新增加的方法吧。

**解决方案：** 这里选择注释掉，为了不影响其他版本的Qt，所以使用Qt版本宏进行注释.
```
 #if (QT_VERSION < QT_VERSION_CHECK(5,9,0))
 
 ......
 #endif
```