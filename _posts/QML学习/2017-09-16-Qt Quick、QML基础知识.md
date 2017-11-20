---
layout : post
title: Qt Quick、QML基础知识
category : QML学习
wangyiyun: true
date : 2017-09-16
---

******

    作者:鹅卵石
    时间:2017年9月16日23:13:12
    版本:V 0.0.2
    邮箱:kevinlq@yeah.net

<!-- more -->


现阶段需要用到Quick和QML基础知识相关的知识，特此学习整理下，做一份笔记.

### Qt Quick和QML区别

**这两个不是同一个东西**

- Qt Quick是一个框架
Qt Quick是使用C++搭建起来的一套UI框架，和之前的QWidgets没有半毛钱关系!
- QML(Qt Meta-Object Language)是一种语言
QML是一种解释性的语言，引入这个语言可以提高开发效率

### 开发工具
- Qt Creator (主要在这里进行编码)
- qmlscene(安装好Qt后直接可以在bin目录下找到，用来直接加载一个qml文件)
- QML Analyzer(QML性能分析器，以后会用到)

### Quick项目类型区别
新建一个Quick工程，发现有好几个选项，这里列举下每个选项的区别以及意义

![](/res/img/blog/QML学习/project_con.png)

主要包含两个`Qt Quick Application`和`Qt Quick Controls Application`.其中`Qt Quick Controls 2 Application`是前者的加强版，应该可以这么说.

- Qt Quick Application

首先新建一个Quick Application工程

新建工程后内容如下：

![](/res/img/blog/QML学习/QuickApplication.png)

可以发现Qt Quick Application是QQuickWindows的一个实例.

- Qt Quick Controls Application

![](/res/img/blog/QML学习/QuickConotrol.png)

Qt Qucik Control 项目中的ApplicationWindow继承自Window，并且提供了原生的菜单栏、状态栏、工具栏.

### 注释
QML中的注释很其他程序设计语言类似，主要有两种:
- 单行注释  
使用双斜杠//
- 多行注释  
使用/*...*/

### 常用组件

**可视化组件**
- Rectangle
- Image
- Text
- TextInput
- Button
- CheckBox
- GroupBox
- Lable
- ProgressBar
- RadioButton
- Slider
- SpibBox
- TextField

**几个非可视化组件**
- ListView
- Transition
- State
- Timer


1. **Rectangle**

Rectangle作为最基本的组件元素，就是一个矩形框，使用该矩形框可以实现各种窗口、面板、按钮.

2. **Image**

Image就是现实图片、图像意思.可以设置透明图片.

3. **Text**

Text就是文本.用来现实文本，可以配合图片进行图文展示.

4. **TextInput**

TextInput就是输入框，用来进行文字输入.

5. **Button使用**

需要引入
```C++
import QtQuick.Controls 1.4
```

### 基本的交互项
- MouseArea(鼠标句柄交互)
- FocusScope(键盘焦点句柄)
- Flickable(浏览图片一部分效果)
- Flipable(提供一个平面，可以进行翻转查看前面或者后面)


### 布局
- 锚点布局
- Grid布局

### 窗口和菜单

#### ApplicationWidow
- 是一个顶级的窗口；
- 类似于QMainWindow,可以设置Menubar,StatusBar,ToolBar
- modality属性可以设置模态和非模态显示

### 视图
- TableView
- TabView
- ScrollView
- SplitView

### 页面导航
- StackView


### 对话框


### 和C++交互


### 无边框

```C++
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    flags: Qt.FramelessWindowHint;

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}
其中
flags: Qt.FramelessWindowHint;实现了无边框效果.

```

## 资源路径加载方式
- 加载Qt程序中的资源

```C++
   FontLoader{
       id:axisDisplayFont
       source:"qrc:/src/font/dig.ttf"
   }
```

- 加载磁盘资源

```C++
Image
  {
      source: "///C:/Users/Administrator/Pictures/0.jpg";

  }
  或者
  Image
  {
      source: "file:///C:/Users/Administrator/Pictures/0.jpg";

  }
```

## 字符处理

- 字符串截取
字符串截取只能使用javascript方法来进行
```QML
    Button {
        id:tbn
        text: "button"

        anchors.top: name.bottom

        property string m_value: "50%"

        onClicked: {
            console.log( Number( m_value.substring(0,2)))
        }
    }
```
上面代码，通过按下按钮可以将字符`50%`转换成数字50


---

如果您对本文有任何问题，可以在下方留言，或者Email我 .微信公众号：devstone，扫描二维码直接关注

<center>
<img src="/res/img/blog/qrcode_for_devstone.jpg" width="25%" height="25%" />
</center>