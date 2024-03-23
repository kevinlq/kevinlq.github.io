---
layout: post
title: QML 高效开发之加载方式
categories: [Qt]
description: 加载 QML 的方式
keywords: Qt, C++
---


## 背景

日常使用 `QML`开发软件过程中，默认使用 `qrc` 存在开发效率慢问题，比如每次修改界面内容后需要先构建才能看到实际效果，尤其在频繁调试界面样式时显得更繁琐

## 原理

`Qt` 中的资源分为两类，`编译型`和`外部二进制资源`

编译型资源每次更改后都需要构建才能生效。`QML` 作为一种特殊的资源文件，每次改动后`qrc`无法感知是否修改，构建后才会更新对应的数据内容(如图所示)

![rcc](/res/img/blog/Qt-learn/qt-rcc.png)


其它资源文件比如图片等，原理类似

比如你在程序中使用了某张图片，需求变化需要更新某张图片，你直接拷贝替换后直接运行程序，发现未生效，找了半天问题，最后构建后发现神奇的好了

## 解决方案

知道了问题原因，那就可以对症下药。简而言之，跳过 编译型 `qrc`方式，直接从本地路经加载 `QML` 资源，比如

- 开发阶段:
```QML
qmlRes = file:///D:/code/src/KQML/App.qm
QQmlApplicationEngine engine;
engine.load(qmlRes);
```

- 正式发布阶段
```C++
qmlRes = "C:/kchat/cfg/KSkinRes.rcc"
QResource::registerResource(qmlRes);
engine.load(qmlRes);
```

> 上面的 `KSkinRes.rcc` 文件是通过 `Qt` 的 `rcc` 工具编译生成的二进制文件

## 完整示例解决方案

`C++` 代码处理

- 增加加载方式配置文件
用来判断判断程序资源走本地模式还是外部二进制模式，配置文件格式根据自己项目而定,`XML`，`JSON`，`ini` 均可，下面以常用的 `ini`为例

本地开发时修改成本地模式
```C++
[General]
skinRes=file:///D:/CloundCode/KWeChatQml/src/KQML/App.qml
```
项目发布时，走 `qrc` 模式
```
[General]
skinRes=qrc:/KQML/App.qml
```
- C++ 加载适配
```C++
    QSettings settings("./conf/app.ini", QSettings::IniFormat);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    settings.setIniCodec("UTF8");
#endif

    if(settings.contains("skinRes")) {
        qmlRes = settings.value("skinRes").toString();
    }
    if(qmlRes.startsWith("file:///", Qt::CaseInsensitive))
    {
        QFileInfo fInfo(QUrl(qmlRes).toLocalFile());
        resPrefix = fInfo.absolutePath() + "/";
    }
    else {
        QString strFullResPath = QCoreApplication::applicationDirPath() + "/conf/KSkinRes.rcc";
        if (!QResource::registerResource(strFullResPath))
        {
            LOG_ERROR() << "QResource registerResource qml failed! " << strFullResPath;
            return 1;
        }
    }

    QQmlApplicationEngine engine;
    engine.load(qmlRes);
```
- 资源编译
```
rcc.exe -binary ./qml.qrc -o ./bin/KSkinRes.rcc
```
其中 `qml.qrc`文件包含了项目中使用到的所有资源文件：图片，`QML`/`js`文件等


## 参考文章

https://doc.qt.io/qt-5/resources.html

******

    作者:鹅卵石
    版本:V 0.0.1
    版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
    希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->

---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**