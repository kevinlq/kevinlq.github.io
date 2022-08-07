---
layout: post
title: Qt 学习笔记
categories: [Qt]
description: Qt 笔记
keywords: Qt, 源码,Qt
---

## QFile 文件属性区区别

```C++
    QString strSourceFile = "D:/Qt/source-target.testFileName.tar.gz";
    QFile file(strSourceFile);

    QFileInfo fileInfo(strSourceFile);

    qDebug() << "file:" << file.fileName();

    qDebug() << "fileName:" << fileInfo.fileName();
    qDebug() << "baseName:" << fileInfo.baseName();
    qDebug() << "filePath:" << fileInfo.filePath();
    qDebug() << "suffix:" << fileInfo.suffix();
    qDebug() << "absolutePath:" << fileInfo.absolutePath();
    qDebug() << "absoluteFilePath:" << fileInfo.absoluteFilePath();
    qDebug() << "absoluteDir-path:" << fileInfo.absoluteDir().path();
    qDebug() << "canonicalFilePath:" << fileInfo.canonicalFilePath();
    qDebug() << "canonicalPath:" << fileInfo.canonicalPath();
    qDebug() << "completeSuffix:" << fileInfo.completeSuffix();
    qDebug() << "dir-path:" << fileInfo.dir().path();
```

输出信息如下：
```C++
file: "D:/Qt/source-target.testFileName.tar.gz"
fileName: "source-target.testFileName.tar.gz"
baseName: "source-target"
filePath: "D:/Qt/source-target.testFileName.tar.gz"
suffix: "gz"
absolutePath: "D:/Qt"
absoluteFilePath: "D:/Qt/source-target.testFileName.tar.gz"
absoluteDir-path: "D:/Qt"
canonicalFilePath: "D:/Qt/source-target.testFileName.tar.gz"
canonicalPath: "D:/Qt"
completeSuffix: "testFileName.tar.gz"
dir-path: "D:/Qt"
```

---
## Q_PROPERTY 自动化生成

只需要定义好 `Q_PROPERTY`，然后光标在该属性上面按下`Ctrl + 回车`，会自动给你创建好对应的成员变量以及 `set` 和 `get` 方法


---

## QML 使用枚举

### C++ 中定义然后注册给 QML 使用

自从 `Qt 5.5`开始，支持在 `C++` 中定义一个枚举，然后使用 `Q_ENUM`定义关键字，最后注册枚举到原对象系统中，这样在 `QML` 中可以直接使用

```C++
class DefineGlobal : public QObject
{
    Q_OBJECT
public:
    explicit DefineGlobal(QObject *parent = nullptr);

    enum StatusType {
        Normal      = 0x0,
        Hovering    = 0x1,
        Pressed     = 0x2,
        Checked     = 0x3
    };
    Q_ENUMS(StatusType)
};
```

然后用注册的方式
```C++
 qmlRegisterType<DefineGlobal>("DefineGlobal.kevinlq.com", 1, 0, "DefineGlobal");

```

然后在 `QML` 中使用

``` QML
import QtQuick 2.15
import QtQuick.Window 2.15
import DefineGlobal.kevinlq.com 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Component.onCompleted: {
        console.log("###StatusType:",DefineGlobal.Checked)
    }
}
```

还有一种方法，不需要定义类对象，直接将定义的枚举类型注册完给 `QML` 使用

```C++
namespace Button_State {
Q_NAMESPACE
enum Button_StateType {
    Normal      = 0x0,
    Hovering    = 0x1,
    Pressed     = 0x2,
    Checked     = 0x3,
};
Q_ENUMS(Button_StateType);
}

// 注册
qmlRegisterUncreatableMetaObject(Button_State::staticMetaObject,"kevinlq.com.devstone",
major,minor,"ButtonState", "Access to enums & flags only");

// QML 使用

ButtonState.Normal
```

### QML 中直接定义

到了 `Qt 5.10`已经支持在 `QML` 中定义枚举了
[官方介绍连接](https://doc.qt.io/qt-5/qtqml-syntax-objectattributes.html#enumeration-attributes)

```js
MyComponent.qml

Rectangle {
    id: root

    // Define Shape enum
    enum Shape {
        None,
        Round,
        Pointy,
        Bobbly,
        Elusive
    }

    // Note: property using enum is of type int
    property int selectedShape: MyComponent.Shape.None

    visible: selectedShape !== MyComponent.Shape.None
    color: selectedShape === MyComponent.Shape.Pointy? "red": "green"
}
```

## 注册 C++对象给 QML 使用

日常在`C++`中使用的对象，如果需要可以注册给 `QML` 来使用，包括类对象、枚举、单例类等，提前是这些类必须继承 `QObject`

**普通对象注册**

```C++
class HandleMessage: public QObject
{
    Q_OBJECT
public:

    enum MsgCode
    {
        MsgRun
    };
    Q_ENUMS(MsgCode)

    HandleMessage(QObject *parent = nullptr) : QObject(parent)
    {
    }
};

// 注册

const char *uri = "kevinlq.com.devstone";
int versionMajor = 1;
int versionMinor = 0;

qmlRegisterType<HandleMessage>(uri,versionMajor,versionMinor,"HandleMessage");
```

使用

```C++
//Main.qml

Rectangle
{
    HandleMessage
    {
        id: msgHandle
    }

    Component.onCompleted: 
    {
        // 使用C++枚举
        console.log("###StatusType:",HandleMessage.MsgRun)
    }
}
```

**单例对象注册**

单例对象比较特殊，不能使用上述方法进行注册
```C++
class HandleMessage: public QObject
{
    Q_OBJECT
public:

    enum MsgCode
    {
        MsgRun
    };
    Q_ENUMS(MsgCode)

    static HandleMessage* getInstance()
    {
        static HandleMessage _instance;
        return &_instance;
    }

private:
    HandleMessage(QObject *parent = nullptr) : QObject(parent)
    {
    }
};

// 注册
// 需要先提供一个静态方法用于返回该对象，注册的时候回调使用
 static QObject *msgHandleProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
 {
     Q_UNUSED(engine)
     Q_UNUSED(scriptEngine)

     return HandleMessage::getInstance();
 }

const char *uri = "kevinlq.com.devstone";
int versionMajor = 1;
int versionMinor = 0;

// Second, register the singleton type provider with QML by calling this 
qmlRegisterSingletonType<HandleMessage>(uri, versionMajor, versionMinor, "HandleMessage", msgHandleProvider);
```

单例对象使用时直接用该对象名即可调用对象成员属性和方法

```C++

Rectangle
{
    // 这种写法是错误的，单例类不需要在这里初始化了
    // HandleMessage
    // {
    //     id: msgHandle
    // }

    Component.onCompleted: 
    {
        // 使用C++枚举
        console.log("###StatusType:",HandleMessage.MsgRun)
    }
}
```


******

    作者:鹅卵石
    时间: 2020年2月14日21:43:20
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="https://gitee.com/devstone/imageBed/raw/master/code/myCode.png" width="50%" height="50%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.