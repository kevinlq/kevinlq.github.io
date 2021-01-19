---
layout: post
title: QLibrary 动态库加载使用
categories: [Qt学习]
description: Qt 在动态库加载时的常用方法
keywords: Qt,QLibrary
---

>阅读本文大概需要 6.6分钟

**更多内容敬请关注公众号：devstone**

<center>
<img src="/res/img/blog/qrcode_for_devstone.jpg" width="40%" height="40%" />
</center>


一般情况下在没有头文件支持情况下，想要引入某个动态库，最好的办法就是使用「动态加载」的方法，在`Qt`中一般使用`QLibyary`来操作

## 常用 api

```c++
 QLibrary(const QString& fileName, QObject *parent = nullptr);
 QFunctionPointer resolve(const char *symbol);
 bool load();
 bool unload();
 bool isLoaded() const;
```

[官方说明](https://doc.qt.io/qt-5/qlibrary.html "官方api说明")

下面我们来看下详细的使用过程

## 创建动态库

我们创建一个「子工程」，用来编写我们的测试方法，该工程需要导出为动态库，完整的示例如下所示

```c++
├─App
│      App.pro
│      main.cpp
│
└─Libs
        Libs.pro
        SDKCore.cpp
        SDKCore.h
        SDKCoreImpl.cpp
        SDKCoreImpl.h
        SDKCore_global.h
        SDKStruct.h
```
我们创建一个函数，并且导出

SDKCore.h

```c++
extern "C" SDK_EXPORT int runFunction(const char * funName, const DS_Request &request, DS_Request& response);
```
SDKCore.cpp

```c++
int runFunction(const char *funName, const DS_Request &request, DS_Request &response)
{
    Internal::SDKCoreImpl m_pCoreImpl;
    int nRet = -1;
    QMetaObject::invokeMethod(&m_pCoreImpl, funName,
                              Qt::DirectConnection,
                              Q_RETURN_ARG(int, nRet),
                              Q_ARG(DS_Request, request),
                              Q_ARG(DS_Request&,response));
    return nRet;
}
```
这里为了访问方便，具体的函数都放在了`SDKCoreImpl`中实现，调用采用了反射，调用的时候直接传方法字符串即可

上面中入参和出参数据格式是自定义的，为了统一而自定义，具体如下所示

```c++
struct DS_Request{
    QString m_strData;
    QByteArray m_baStream;
};
```
这样就兼容所有的格式了，不管是传 字符串还是数据流都使用统一的格式

这里写一个简单的加法运算

```c++
int SDKCoreImpl::addNum(const DS_Request &request, DS_Request &response)
{
    QVariantMap map = QJsonDocument::fromJson(request.m_strData.toLatin1()).toVariant().toMap();
    int nValue1 = map.value("nValue1").toInt();
    int nValue2 = map.value("nValue2").toInt();
    int nResult = nValue1 + nValue2;
    QVariantMap mapResult;
    mapResult.insert("result", nResult);
    QJsonDocument doc = QJsonDocument::fromVariant(mapResult);
    response.m_strData = QString(doc.toJson());
    return SDKCODE_SUCCEED;
}
```
## 加载动态库

加载动态库就变的很简单了，只需要几行代码即可获取到对应的函数符号

```c++
 QLibrary library(strSDKName);
 bool bLoad = library.load();
```
只要动态库路径合法就会加载成功，一般加载失败可能就是路径后者导出符号有问题，遇到这些问题可以排查下

使用前先定义一个函数指针

```c++
typedef int (* pRunFunctionFun)(const char *, const DS_Request &, DS_Request&);
```
调用`addNum`函数

```c++
pRunFunctionFun pFun = (pRunFunctionFun) library.resolve("runFunction");
if ( pFun)
{
    int nRet = pFun("addNum", request, response);
    qDebug() << "add Resut:" << nRet << response.m_strData;
}
```
这样一个完整的调用流程就跑通了

## 优化代码

但是，如果你认为完了，那就错了，仔细看下就会发现上述代码有冗余，难道每调用不同的函数，都要重复上述过程么？肯定不会呀，程序员不会自己干重复的事情，要重复只能让机器去做

下面优化下，把重复的代码单独封装下，定义成宏

通过上述代码，我们可以看出来，首先这个方法可以封装成模板，每一次调用都是一样的

```c++
library.resolve("")
```
封装成模板调用

```c++
template <typename T>
T getFunction(QLibrary *lib, const char *symbol)
{
    T f = (T)lib->resolve(func);
    if (f == nullptr)
    {
        return nullptr;
    }
    return f;
}
```
然后接着优化，获取函数符号流程封装成宏

```c++
#define GET_SDK_FUNC(type, func, symbol)  GET_FUNC(sdk, type, func, symbol);
#define GET_FUNC(sdk, type, func, symbol) \
    type func = getFunction<type>(&sdk, symbol); \
    if (func == nullptr) \
{ \
    qDebug()<<"fun is null: "<< symbol; \
    return 0; \
}
```
优化完成后，我们看下调用过程会如何更简单

```c++
QLibrary library(strSDKName);
bool bLoad = library.load();
GET_FUNC(library, pRunFunctionFun, runFun, "runFunction");
int nRet = runFun("addNum", request, response);
```
完整代码已经上传到`GitHub`，感兴趣的可以自行去下载

[demo 源码下载](https://github.com/kevinlq/DynamicLibLoadDemo "示例源码下载地址")

## 总结

在平时工作中，很多大型工程都是动态库调用，这样做的好处就是方便他人直接调用使用现有功能和业务，减少重复复杂的工作，提高我们的工作效率

上述代码中有没有发现一条迷惑之处，就是动态库的路径和名称没有看到呀，是怎么加载的？

这个其实用到了之前介绍的一种技术,`qmake`语法糖

阅读过源码你就会发现，我在`pri`文件中编写了字符串宏定义，这样在代码中就可以直接调用对应的字符串了，如果那一天要修改，那么只修改`pri`文件即可，其它都不需要动

```c++
DEFINES += LIB_DIR_PATH=\"\\\"$$LIB_DIR_PATH\\\"\"
DEFINES += FILE_DLL_EXT=\"\\\"$$FILE_DLL_EXT\\\"\"
DEFINES += LIB_NAME=\"\\\"$$LIB_NAME\\\"\"
```




******

    作者:鹅卵石
    时间: 2021年1月19日22:49:59
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 更多内容敬请关注公众号：devstone

<center>
<img src="/res/img/blog/qrcode_for_devstone.jpg" width="50%" height="50%" />
</center>

---
