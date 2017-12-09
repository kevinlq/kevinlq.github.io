---
layout: post
title: Windows下MinGW跨平台编译和使用log4cpp
categories: [开源库]
description: 编译log4cpp
keywords: Qt,log4cpp,日志
---

`Log4cpp` 是C++开源日志库，为 C++ 应用程序开发中提供了日志的追踪和调试功能，基于 `LGPL` 开源协议，移植自 java 的日志项目 log4j, 并在 api 上保持了一致性。

## 1. 环境

- windows 7 64b + Qt5.7 + MinGW530
- log4cpp: 1.1.3

## 2. 下载

- 下载地址：[https://sourceforge.net/projects/log4cpp/](https://sourceforge.net/projects/log4cpp/)
- log4cpp主页:[http://log4cpp.sourceforge.net/](http://log4cpp.sourceforge.net/)

## 3.特点
- 可扩展，提供了多种记录方式：文件、命令行、内存、win事件日志...
- 可以动态控制日志级别，进行动态调整
- 多语言支持,C(log4c), C++(log4cpp/log4cplus), java(log4j), python(log4p))
- 平台无关性，源码编译后可以运行在各大主流操作系统上


## 4. 编译

打开工程后，可以发现包含了多个工程编译项目，不过基本都是基于VS的项目，不是我想要的，需要手动转化成 Qt 项目，也就是编程 `pro` 形式。


拷贝源码目录下的 `include` 和 `Src` 文件夹到自己新建的工程中，主要使用这两部分，其余的先不管。

**编写 pro 文件**

```C++
QT       = core

TEMPLATE = lib
TARGET = log4cpp
INCLUDEPATH += $$PWD/.. \
               $$PWD/../../boost/ \
DEPENDPATH += $$PWD/..

DEFINES += LOG4CPP_HAVE_BOOST

SOURCES += \
	Appender.cpp \
	AppenderSkeleton.cpp \
	AppendersFactory.cpp \
	......

HEADERS += \

win32:{	LIBS += -L -lwsock32 -lws2_32	}

```
上述引入了 `boost 库路径，并添加了wsock32库！否则编译会出现错误。

编译成功后会生成对应的库文件(静态库只有629KB):

![](/res/img/blog/3rdparty/log4cpp_lib.png)


## 5. 使用

### 5.1 基本概念

- Catgory (种类)
- Appender (附加目的地)
- Layout (布局)
- Priority (优先级)
- DNC (嵌套的诊断上下文)

#### 5.1.1 Category

`Category` 级别，优先级可以用来控制日志输出的数量，比如配置级别是 DEBUG ,则任意 log都可以被打印出来，如果配置级别是 ERROR，则只有高于这个级别的才能输出.

`Category` 真正完成了日志记录功能.

优先级: DEBUG < INFO < WARN < ERROR < FATAL

#### 5.1.2 Appender

`Appender`(输出源) 负责将 layout 附加的消息 记录到某个输出设备上(比如：文件，socket).

多个 `Appender` 可以附加到一个 `Category` 上，这样一个消息就能同时输出到多个设备上

**Appender设备:**

- log4cpp::IdsaAppender           // 发送到IDS或者logger, 详细见 http://jade.cs.uct.ac.za/idsa/  
- log4cpp::FileAppender           // 输出到文件  
- log4cpp::RollingFileAppender    // 输出到回卷文件，即当文件到达某个大小后回卷  
- log4cpp::OstreamAppender        // 输出到一个ostream类  
- log4cpp::RemoteSyslogAppender   // 输出到远程syslog服务器  
- log4cpp::StringQueueAppender   // 内存队列  
- log4cpp::SyslogAppender         // 本地syslog  
- log4cpp::Win32DebugAppender     // 发送到缺省系统调试器  
- log4cpp::NTEventLogAppender     // 发送到win 事件日志 

#### 5.1.3 Layout

layout 类即布局，其实就是用来控制日志消息以怎么样的格式显示(这些开源库中类名比较怪，就不能直接点么，不就是日志格式么，搞一个 layout )。主要有一下几种格式:

- `log4cpp::BasicLayout`
时间戳 + 优先级 + 类别 + NDC 标签 + 日志消息

- `log4cpp::PatternLayout`
类似C语言中的 printf 格式化输出,可以指定格式输出

- `log4cpp::SimpleLayout`
优先级 + 日志信息

以上日志格式不太满意，所以需要对其进行升级(自定义)，没有办法，世上很难遇上自己满意的东西，遇上喜欢的还要自定义。以下格式应该算是大部分人能接收到的格式：

```C
[2017-12-03 15:10:50:393|ERROR : [fun: int main(int, char**)] [line:32] [time: "15:10:50.389" ] Msg: error test
```

一般的日志消息需要包含时间戳，消息类型，以及打印该消息的具体文件中函数名称、行数，消息内容。这样的结构基本可以满足日常开发需求。

继承关系:  

![](/res/img/blog/3rdparty/log4cpp_layout.png)

#### 5.1.4 Priority

优先级在 Category中描述过.


#### 5.1.5 DNC

暂时不了解

### 5.2 配置文件

使用 log4cpp 有两种方式，一种是自己手动编写配置步骤，比较繁琐，另一种是直接通过配置文件即可完成.

![](/res/img/blog/3rdparty/log4cpp_config.png)

### 5.3 使用

由于 log4cpp 类名较长，操作不便，所以进行二次分装比较方便

- 读取配置文件
- 实例化 category对象
- 封装日志输出方法

#### 5.3.1 读取配置文件

```C++
bool Log4CppUtility::loadConfigFile(QString strConfigFile, QString strParentPath)
{
    try
    {
        QByteArray dataConfigFile = strConfigFile.toLocal8Bit ();
        QByteArray dataParentPath = strParentPath.toLocal8Bit ();

        log4cpp::PropertyConfigurator::configureEx ( dataConfigFile.data (),
                                                     dataParentPath.data ());
    }
    catch ( log4cpp::ConfigureFailure & e)
    {
        log4cpp::Category::getRoot ().warn (e.what ());
//        qDebug()<<"configureEx problem:"<<e.what ();
        return false;
    }

    return true;
}
```

#### 5.3.2 实例化Category

```C++
bool Log4CppUtility::outputLog(xx,xxx,xxx,...)
{
    log4cpp::Category & category = strCategoryName.isEmpty ()?
                log4cpp::Category::getRoot ():
                log4cpp::Category::getInstance ( strCategoryName.toStdString () );
    //日志等级区分
    switch (Level) {
    case LP_EMERG:
    {
        category.emerg ( strMsg.toStdString () );
        bOutputMsg = category.isEmergEnabled ();
    }
        break;
    case LP_ALERT:
    {
        category.alert ( strMsg.toStdString () );
        bOutputMsg = category.isAlertEnabled ();
    }
        break;
     
     //其他类似......
}
```

#### 5.3.1 日志输出方法

编写一个宏定义

```C++
#define LOGERROR(format, ...) Log4CppUtility::formateLog(__FILE__, __PRETTY_FUNCTION__, __LINE__, CATEGORYNAME, Log4CppUtility::LP_ERROR,  format, ##__VA_ARGS__);

//其他类似...

```

**调用:**

```C++
#include <QDebug>
#include <QCoreApplication>
#include "loginclib.h"

#include <QThread>

class MyThread:public QThread
{
public:
    MyThread()
    {
        m_nCount = 10000;
    }
    ~MyThread(){}

protected:
    virtual void run()
    {
        while (m_nCount)
        {
            LOGERROR("error:"+QString::number (m_nCount));
            m_nCount--;

            msleep (4);
        }
    }

private:
    int m_nCount;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc ,argv);
    qDebug()<<"=====start test log=====";

    QString strFilePath = QCoreApplication::applicationDirPath ()+"/";

    QString logConfig = "LogProperty.conf";
    logConfig = strFilePath + logConfig;

    if ( !Log4CppUtility::loadConfigFile ( logConfig ))
    {
        qDebug()<<"set path error!";
        return 0;
    }

    MyThread thread;
    thread.start ();

    qDebug()<<"=====end test log=====";


    int ret = app.exec ();

    Log4CppUtility::shutDown ();

    return ret;
}

```

新建一个线程不断的输出日志到文件(这里只显示5条记录).

使用是很方便的:

```C++
LOGERROR("this is error info");
```

实际结果:

![](/res/img/blog/3rdparty/log4cpp_result.png)
