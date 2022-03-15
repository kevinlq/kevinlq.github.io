---
layout: post
title: Qt 学习笔记
categories: [Qt]
description: Qt 笔记
keywords: Qt, 源码,Qt
---

## QFile 文件属性区区别



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

### QML 中直接定义

到了 `Qt 5.10`已经支持在 `QML` 中定义枚举了
[官方介绍连接](https://doc.qt.io/qt-5/qtqml-syntax-objectattributes.html#enumeration-attributes)

```js
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