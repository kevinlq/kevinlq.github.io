---
layout : post
title: QML文件加载的几种方式
category : QML学习
wangyiyun: true
date : 2017-09-26
---

******

    作者:鹅卵石
    时间:22017年9月26日21:07:25
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net

<!-- more -->

学习QML过程，看到好多方式加载QML文件，这里参考网上总结整理下.

## QQmlApplicationEngine加载QML

查看帮助文档，可以看出`QQmlApplicationEngine`继承自`QQmlEngine`->`QObject`.

这种方式属于`QQmlApplicationEngine`搭配`Window`

**main.cpp**
```C++
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
```
对应QML文件内容:
**main.qml**
```C++
import QtQuick 2.7
import QtQuick.Window 2.2

Window {
    visible: true
    width: 460
    height: 320
    title: qsTr("QQmlApplicationEngine方式加载")

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}
```
**效果**

![](/res/img/blog/QML学习/QQmlApplicationEngine.png)

>以上方式加载以`Window`为根对象的QML文件，这个时候QML就完全拥有了控制权，可以直接设置窗体的标题、尺寸等信息.


## QQuickView加载QML

**main.cpp**
```C++
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl("qrc:/main.qml"));
    view.show ();

    return app.exec();
}
```
这个时候需要修改QML文件中的内容了，如果不修改，直接运行，则会出现2个窗体，并且会输出警告:

![error](/res/img/blog/QML学习/QQuickView.png)

```
QQuickView does not support using windows as a root item. 

If you wish to create your root window from QML, consider using QQmlApplicationEngine instead. 
```

错误很明显，提示不能使用windows作为根.

这里需要将`window`替换成`Rectangle`.替换后直接运行，你就会发现提示`qrc:/main.qml:8:5: Cannot assign to non-existent property "title"`.意思很明确了，这里不能通过QML来设置窗口的标题了.

使用这种方式时，对窗口的控制权在C++代码中要实现，QML文件是以Item作为根对象的.

**main.qml**
```C++
import QtQuick 2.7
import QtQuick.Window 2.2

Rectangle {
    visible: true
    width: 460
    height: 320
    //title: qsTr("QQmlApplicationEngine方式加载")

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}
```

需要设置标题时可以在C++中来实现,只需要在`main.cpp`中添加一行:

```
view.setTitle ("QQuickView方式加载");
```

## QQuickWidget加载QML

官网自带的说明例子.一般用来在QWidget界面上加载QML界面，不过我觉得一般人是不会这么干的，况且这样使用不觉得累么!

```C++
    QQuickWidget *view = new QQuickWidget;
    view->setSource(QUrl::fromLocalFile("myqmlfile.qml"));
    view->show();
```



---

如果您对本文有任何问题，可以在下方留言，或者Email我 .微信公众号：devstone，扫描二维码直接关注

<center>
<img src="/res/img/blog/qrcode_for_devstone.jpg" width="25%" height="25%" />
</center>