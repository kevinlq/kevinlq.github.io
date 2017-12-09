---
layout: post
title: Windows下MinGW跨平台编译和使用QxOrm 
categories: [开源库]
description: 编译QxOrm
keywords: Qt，QxOrm
---


QxOrm 是一个基于QT开发的数据库方面的ORM库,功能很强大.是QT C++数据开发方面的好工具。

官网 [http://www.qxorm.com/](http://www.qxorm.com/) ，目前进不去了。

## 环境

- windows 7 64b + Qt5.7 + MinGW530
- QxOrm:1.4.1

## 编译

QxOrm使用到了boost，第一步需要编译boost，本次使用QxOrm1.4.1，这个版本的不需要再次编译boost，直接引入即可

- 修改QxOrm.pri中boost路径

```C++
QX_BOOST_INCLUDE_PATH = $$PWD/../../boost
QX_BOOST_LIB_PATH = $$PWD/../../boost/platform/$${DIR_PLATFORM}/$${DIR_COMPILER}/$${DIR_COMPILEMODE}
```


## 错误1

![](/res/img/blog/tools/QxOrm_build_error.png)

找不到boost库了，因为编译这个必须先编译boost,这里直接修改成编译好的库的路径

解决方案(QxOrm.pri):
```
isEmpty(QX_BOOST_INCLUDE_PATH) { QX_BOOST_INCLUDE_PATH = $$quote($$(BOOST_INCLUDE)) }
修改成:
isEmpty(QX_BOOST_INCLUDE_PATH) { QX_BOOST_INCLUDE_PATH = $$PWD/../../boost}
后面出现的依赖库路径都需要修改。
最后的静态编译宏定义也要添加
 DEFINES += _QX_STATIC_BUILD
```

## 错误2
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

## 使用