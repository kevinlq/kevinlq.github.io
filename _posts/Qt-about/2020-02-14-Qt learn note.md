---
layout: post
title: Qt 学习笔记
categories: [Qt]
description: Qt 笔记
keywords: Qt, 源码,Qt
---

## 文件和文件夹相关操作

### QFile 文件属性区区别

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
### QDir 常见使用

`QDir` 常用在文件夹操作，提供了文件夹的增、删、改，常见的操作主要有：

- path: 返回当前 dir 的路径
- currentPath: 返回当前程序 exe 的路径
- cdUp: 会返回到上一级目录(只有文件路径存在时返回成功，文件路径不存在返回失败)

```C++
    QString path("D:/Qt/Qt5.9.0/5.9/msvc2013_64/bin");
    QDir dir(path);

    qDebug() << "path:" << dir.path();
    qDebug() << "currentPath:" << dir.currentPath();
    qDebug() << "dirName:" << dir.dirName();

    bool bResult = dir.cdUp();

    qDebug() << bResult;
    qDebug() << "path:" << dir.path();
```

输出信息如下：
```
path: "D:/Qt/Qt5.9.0/5.9/msvc2013_64/bin"
currentPath: "D:/Project/untitled5/temple"
dirName: "bin"
true
path: "D:/Qt/Qt5.9.0/5.9/msvc2013_64"
```

### 打开资源管理器并定位到指定文件

- 方法1 QDesktopServices::openUr

有局限，仅仅能打开文件/文件夹，不能定位到

- 方法2 QProcess 调用 explorer

使用时注意空格等特殊路径问题，最好转换下

核心命令:
```
explorer.exe /select,"D:/Qt/Qt5.9.0/5.9/msvc2013_64/bin/qmlscene.exe"
```

```
    QString srcFile = "D:/Qt/Qt5.9.0/5.9/msvc2013_64/bin/qmlscene.exe";
    QString program = "explorer";
    QStringList arguments;
    arguments << "/select,"
              << QDir::toNativeSeparators(srcFile);

    QProcess::startDetached(program, arguments);
```

这个其实是利用了 `explorer.exe` 来执行一些命令，还有其他可用的命令可以使用：

Explorer [/n][/e][[,/root],[path]][[,/select],[path filename]]

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

## ListView 注意事项

- 平滑滚动+取消拖动反弹效果
```
interactive: true
boundsMovement: Flickable.StopAtBounds
```
> interactive 默认为 true ，如果设置为 false， 则整个View 不会随着鼠标滚动而进行翻页显示， 也就失去了翻页功能，一般不更改整个属性，通过设置其它属性来达到目的

默认我们可以拖动 view 到最顶部、最底部，此时会有一个反弹效果，很多时候是不需要这个效果的，可以通过 `boundsMovement` 属性来控制

```
Flickable.StopAtBounds: 取消反弹效果
Flickable.FollowBoundsBehavior：  默认效果
```

原文
``` 
Flickable.StopAtBounds - the contents can not be dragged beyond the boundary of the flickable, and flicks will not overshoot.
Flickable.DragOverBounds - the contents can be dragged beyond the boundary of the Flickable, but flicks will not overshoot.
Flickable.OvershootBounds - the contents can overshoot the boundary when flicked, but the content cannot be dragged beyond the boundary of the flickable. (since QtQuick 2.5)
Flickable.DragAndOvershootBounds (default) - the contents can be dragged beyond the boundary of the Flickable, and can overshoot the boundary when flicked
```

## QML 绘制的三种方式

- 继承 QQuickPaintedItem 重写void paint(QPainter *painter)
- 继承 QQuickItem 重写 QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
- Canvas 绘制


## QObject 派生类不在.h声明报错问题

一般我们继承 `QObject` 总是写在 `.h` 头文件中，有时候也写在 `.cpp` 文件中，这个时候就会报错，原因是 `moc` 工具无法识别我们 `cpp` 中写的内容，因此需要我们手动引入来让其识别

下面以 `main.cpp` 为例子
```

class DS_Student: public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, StuName)
    AUTO_PROPERTY(int, StuAge)
public:
    DS_Student() {;}
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DS_Student stu;
    stu.setStuName("admin");
    stu.setStuAge(12);

    return a.exec();
}

#include "main.moc"
```

关键就是最后一句引入`xx.moc`，添加后清除项目、qmake、重新构建即可正常编译通过


## Q_D,Q_Q 使用模板
```C++

KSerialize.h

class KSerialize : public QObject
{
	Q_OBJECT
public:
    explicit KSerialize(QObject *parent = nullptr);
	
private:
    Q_DECLARE_PRIVATE(KSerialize)
};

KSerialize.cpp

KSerialize::KSerialize(QObject *parent)
    : QObject{*new KSerializePrivate{}, parent}
{
    //Q_D(KSerialize);
}

调用 D指针前：
Q_D(const KSerialize);

KSerializePrivate.h

class KSerializePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(KSerialize)
public:
}
```

如果是多继承方式，还需要增加保护的构造函数

```C++
protected:
	explicit KSerializerBase(QObject *parent = nullptr);
	explicit KSerializerBase(KSerializerBasePrivate &dd, QObject *parent);
	
这样派生类就可以初始化自己的 D指针.
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