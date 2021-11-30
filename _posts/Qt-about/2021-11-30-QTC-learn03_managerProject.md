---
layout: post
title: Qt Creator 源码学习笔记03，大型项目如何管理工程
categories: [Qt]
description: Qt Creator 源码学习笔记 03
keywords: Qt, 源码,Qt Creator
---

> 阅读本文大概需要 6 分钟

一个项目随着功能开发越来越多，项目必然越来越大，工程管理成本也越来越高，后期维护成本更高。如何更好的组织管理工程，是非常重要的

今天我们来学习下 `Qt Creator` 是如何组织管理这么庞大的一个项目工程的

## QMake 多工程管理方法

我们知道 `Qt` 采用 `qmake`语法进行组织管理工程结构，想要更好的学习管理一个工程需要你了解基本的`qmake`语法

在`Qt`当中，一般以`xx.pro`结尾的文件是某个工程文件，我们只要打开该文件即可打开该文件管理的工程

### 单工程基本用法

比如我们新建一个`MainWindow`工程，那么自动会生成如下结构的工程文件

![](https://gitee.com/devstone/imageBed/raw/master/images/202111222257476.png)

其中`untitled.pro`文件内容如下所示

```
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = TestDemo
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

```

该文件描述了这个工程一些基本信息

- QT += 表示需要包含哪些模块
- greaterThan 可以判断 Qt 的一些版本进而做一些版本之间差异的事情
- TEMPLATE 表示该工程编译完最终会连接生成一个app，即会生成`xx.exe`可执行文件
- TARGET 表示该工程最终生成的文件名字，如果没有配置默认取该工程名字

那么如果我们想编译生成一个动态库或者静态库该怎么办？关键语句`TEMPLATE`来进行控制

```
TEMPLATE = lib
```

此时编译该工程默认会生成动态库

```
TEMPLATE = lib

CONFIG += staticlib
```
此时编译该工程又会生成静态库，所以关键地方就在上面两句配置


### 多工程用法

多工程项目中一般是某些核心模块编译成库(静态库、动态库)，然后在依赖的地方进行引入即可

比如我们有一个字符串处理工具模块`StringUtil`，该模块最终编译完会生成一个动态库`StringUtil.dll`，然后我们其中一个模块需要使用到该模块，那么该工程怎么使用呢？

首先是工程`pro`文件需要导入该动态库，这样才能加载进来参与编译，否则会提示某些函数未定义的错误

```C++
LIBS    += -L$$PWD/../../ -lStringUtil
```

## 插件依赖管理缘由

上述代码正常情况下是没有任何问题的，但是，但是，但是

凡是重要的事情肯定要说三遍，你以为这样写就完事了，那么说明你的这个库被依赖的工程比较少，如果这个基础库在 n 个工程下面要使用呢？

到这里相比有些人就说了，哪个工程要使用直接复制上述代码过去不就行了，这样做从功能上来看是没有问题，可以正常使用和运行，但是你想过没有，未来的那一天因为特殊原因这个库有变化(名字、路径等等)，你是不是得修改这 n 个地方使用该库的地方呀

程序员碰见重复的代码肯定是不可以忍受的，肯定要想办法封装一下，将修改减少到最小，那么怎么实现比较好呢？

核心思想就是把依赖导入相关流程使用循环搞定，工程初始化时(也就是qmake)自动根据某个规则加载你的所有依赖即可

### 管理方案

当你阅读 `Qt Creator` 源码的时候就可以看到比较有意思的写法，每个插件或者动态库都有自己的依赖配置`pri`文件，该文件记录了这个库或插件依赖那些库、那些插件

每个插件的依赖配置文件命名类似这样：`xxx_dependencies.pri`，我们拿欢迎界面插件来举个栗子分析下

打开该文件 `welcome_dependencies.pri`。，查看文件内容
```
# 插件名字
QTC_PLUGIN_NAME = Welcome

# 插件依赖的库
QTC_LIB_DEPENDS += \
    extensionsystem \
    utils
    
# 插件依赖的插件
QTC_PLUGIN_DEPENDS += \
    coreplugin
```    
- `QTC_PLUGIN_NAME` 表示了当前生成动态库或者插件的名字
- `QTC_LIB_DEPENDS` 表示当前库依赖的库文件名称，多个库依次追加即可
- `QTC_PLUGIN_DEPENDS` 表示当前插件依赖的插件名称，比如`welcome`插件依赖核心`Coreplugin`插件


那么这些定义的文件是怎么加载进来的？又是怎么起作用的呢？源码面前了无秘密，我们打开`qtcreator.pri`文件来一探究竟，重点关注 244 行到 277 行之间的内容，可以看到如下内容：

```
!isEmpty(QTC_PLUGIN_DEPENDS) {
    LIBS *= -L$$IDE_PLUGIN_PATH  # plugin path from output directory
    LIBS *= -L$$LINK_PLUGIN_PATH  # when output path is different from Qt Creator build directory
}

# recursively resolve plugin deps
done_plugins =
for(ever) {
    isEmpty(QTC_PLUGIN_DEPENDS): \
        break()
    done_plugins += $$QTC_PLUGIN_DEPENDS
    for(dep, QTC_PLUGIN_DEPENDS) {
        dependencies_file =
        for(dir, QTC_PLUGIN_DIRS) {
            exists($$dir/$$dep/$${dep}_dependencies.pri) {
                dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
                break()
            }
        }
        isEmpty(dependencies_file): \
            error("Plugin dependency $$dep not found")
        include($$dependencies_file)
        LIBS += -l$$qtLibraryName($$QTC_PLUGIN_NAME)
    }
    QTC_PLUGIN_DEPENDS = $$unique(QTC_PLUGIN_DEPENDS)
    QTC_PLUGIN_DEPENDS -= $$unique(done_plugins)
}

```

> 上述代码核心思想就是循环获取依赖库文件，然后进行引入

第一句 `for(ever)`是一个无限循环，相当于是死循环`while(1)`，等到 `break` 语句才会退出

第二个循环获取`$$QTC_PLUGIN_DEPENDS`值挨个进行遍历，这个循环是为了检测引入每个依赖插件

第三个循环，首先会判断对应的依赖描述文件是否存在，如果不存在则会输出错误信息给与提醒，后面会`include` 进来该文件，最后使用我们熟悉的`LIBS+=`进行进入库文件

循环最后面两句非常重要，这两句起到停止循环作用，`-=`每次会从`QTC_PLUGIN_DEPENS`中去重`done_plugins`变量对应的插件，最后直到`QTC_PLUGIN_DEPENDS`为空退出最外边的循环

> 上面就是插件依赖处理流程，动态库依赖处理流程原理也类似，比如下面所示

```
done_libs =
for(ever) {
    isEmpty(QTC_LIB_DEPENDS): \
        break()
    done_libs += $$QTC_LIB_DEPENDS
    for(dep, QTC_LIB_DEPENDS) {
        include($$PWD/src/libs/$$dep/$${dep}_dependencies.pri)
        LIBS += -l$$qtLibraryName($$QTC_LIB_NAME)
    }
    QTC_LIB_DEPENDS = $$unique(QTC_LIB_DEPENDS)
    QTC_LIB_DEPENDS -= $$unique(done_libs)
}
```

可以看到整个过程几行代码就可以解决整个项目工程之间的依赖问题，后面开发其它插件和模块只需要按照这个规则编写对应`xx__dependencies.pri` 文件即可，后续的依赖加载会自动处理，可以减少很多工作量以及出错问题

### 小试牛刀

我们来验证下，编写一个工具集模块`Misc`，该模块编译后生成一个动态库，为了尽可能的简单，工程结构如下所示

![](https://gitee.com/devstone/imageBed/raw/master/images/202111222324217.png)

![](https://gitee.com/devstone/imageBed/raw/master/images/202111272321632.png)

文件`Misc_dependencies.pri`列举了该库的依赖信息

```
QTC_LIB_NAME = Misc

```

demo 全部工程源码下载可以访问这里[https://github.com/kevinlq/Qt-Creator-Opensource-Study]


## 多工程管理

上面提到了多工程依赖库的一些管理方法，下面来看看`Qt Creator`工程中一些其它管理技巧

### 善于定义变量

工程中难免会有很多重复的配置，比如：

- 某些文件编译时生成的临时文件路径、编译后动态库、静态库、插件的路径；
- 动态库、静态库命名怎么区分；
- `Debug` 和 `Release` 版本下每个库生成后的名字怎么区分
- 程序版本号怎么在工程配置，然后代码中直接使用

其实稍微大一点的项目，会面临很多基础的问题，解决这些问题要**善于定义变量**

这句话怎么理解呢？我们来看一些基本的例子就明白了

**Debug 和 Release 区分**

有时候我们不同编译模式下生成的库是不一样的，调用第三方库的时候也要注意这一点，那么就要求程序在不同模式下编译后生成的库路径放到不同路径中

```
CONFIG(debug, debug|release):{
    DIR_COMPILEMODE = Debug
}
else:CONFIG(release, debug|release):{
    DIR_COMPILEMODE = Release
}

DESTDIR = $$PWD/$$DIR_COMPILEMODE
```

上面配置会在当前目录下对应编译模式下生成对应库


**文件生成路径定义**

项目中一般都会定义文件的输出路径，比如我有一个动态库要输出指定目录，那么对应的 `pro` 文件该怎么写呢？

```
IDE_APP_NAME = QTC

isEmpty(IDE_BASE_BIN_PATH): IDE_BASE_BIN_PATH = $$QTC_PREFIX/$$IDE_APP_NAME

IDE_LIBRARY_PATH = $$IDE_BASE_BIN_PATH/bin
IDE_PLUGIN_PATH  = $$IDE_BASE_BIN_PATH/$$IDE_LIBRARY_BASENAME/$$IDE_APP_TARGET/plugins
IDE_DATA_PATH    = $$IDE_BASE_BIN_PATH/share/$$IDE_APP_TARGET
IDE_DOC_PATH     = $$IDE_BASE_BIN_PATH/share/doc/$$IDE_APP_TARGET
IDE_BIN_PATH     = $$IDE_BASE_BIN_PATH/bin

```

上述配置命令，在我们程序编译后最终生成的路径格式如下所示：

```
bin
    │  └─Win32
    │      ├─Debug
    │      │  └─QTCLearn03
    │      │      └─bin
    │      │              libMiscd.a
    │      │              libPluginsd.a
    │      │              Miscd.dll
    │      │              Pluginsd.dll
    │      │              QTCLearn03.exe
    │      │
    │      └─Release
    │          ├─QTC
    │          │  └─bin
    │          └─QTCLearn03
    │              └─bin
```

自动根据当前是 `Debug` 还是 `Release`模式生成到对应目录，对程序不会造成干扰，而且每个模块插件可以单独设置其路径，这样做的好处就是分离清晰明确，便于管理和维护

针对插件和动态库可以分别处于不同的目录，以依葫芦画瓢即可完成

> PS： 如果想要完整的 `pro` 配置模板可以私信我，真的很好用，拿来就可以直接用。
事实上，上述代码你也可以学习完`Qt Creator`后自己也可以整理出来。


### Qt Creator源码工程结构

![](https://gitee.com/devstone/imageBed/raw/master/images/202111272358615.png)

源码虽然看起来很多很复杂，不过大概可以分为三个部分，`libs` 、`plugins`，`App`。如上图我重点标红的内容，我在上一篇学习笔记当中介绍过这三个部分分别是干什么的，详细可以看上篇文章 

[Qt Creator 源码学习笔记02，认识框架结构结构](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484676&idx=1&sn=c1bd8cbd78d8e098c22353f567cc3620&chksm=e85c0b34df2b822288a7baa016f4a64df47e5e119039836ff686cb2ad3a7e28a5808efc0c13a&token=1648903909&lang=zh_CN#rd)

`libs` 工程封装了一些外部使用的方法和函数，以动态库的方式呈现，调用时引入动态库加入头文件即可。具体是怎么加入的呢？阅读源码你发现其它子工程并没有直接引入，关键点还是上面提到的**依赖管理方法**

每个子工程都有自己的依赖配置文件，比如`aggregation_dependencies.pri`,这个文件必须要有，否则编译时会报错，提醒你缺少对应的依赖文件

比如核心插件管理库依赖配置 `extensionsystem_dependencies.pri`

```
QTC_LIB_NAME = ExtensionSystem
QTC_LIB_DEPENDS += \
    aggregation \
    utils

```

你能很清晰的看出来这个库依赖两个库，那么在编译它时这两个库必须先编译


`plugins`是所有插件功能的实现部分，包含核心插件以及剩余的扩展插件

插件也是一个个的动态库，只不过每个插件都是继承自同一个接口或者说叫纯虚类，各自实现一些必要的初始化函数，这样才能统一管理和访问控制

工程管理原理还是一样的手法，每个插件都拥有一个配置文件，比如核心插件 `coreplugin_dependencies.pri`

```
QTC_PLUGIN_NAME = Core
QTC_LIB_DEPENDS += \
    aggregation \
    extensionsystem \
    utils

```

可以看出来该插件依赖三个动态库，那么如果插件要依赖呢，怎么写？也非常简单，只需要添加依赖的插件名字即可，比如「书签」插件依赖配置文件

```
QTC_PLUGIN_NAME = Bookmarks
QTC_LIB_DEPENDS += \
    extensionsystem \
    utils
QTC_PLUGIN_DEPENDS += \
    projectexplorer \
    coreplugin \
    texteditor
```

可以看出多了一项`QTC_PLUGIN_DEPENDS`，需要依赖那些插件只需要往后追加即可

这里说到书签插件，是一个非常好用的功能，平时编写调试代码梳理流程非常有用，比如阅读到某个关键函数，发现这个函数又调用了其它文件的方法，跳过去后发现又调用其它功能函数，每次跳转此时太多，想要返回初始位置查看就不方便，有了书签随时点击书签就可以跳转回去了

PS：建议大家使用最新版本的`Qt Creator`，书签会一直缓存，直到你手动删除了，有时候标记了书签，但是下班后电脑关机了第二天打开后发现书签还在，直接开始干活，效率非常高

`app`工程是程序主入口，会显示加载三个动态库，各个插件的调用是在`main.cpp`函数里面动态加载调用的


当然了，实际配置时你还要考虑各各个平台下的一些兼容性，比如 `mac`、`linux`等平台，如果你的软件不涉及这些平台那么就不用考虑了


## 总结

通过阅读开源的框架和项目，可以增加我们的见识，平时工作或者学习当中可能不注意或者想不到的一些技巧方法和编程思想在阅读源码的过程中都可以看到，时间久了各方面都会有很大的提升

下一篇我们来学习看看`Qt Creator`核心插件管理机制是如何实现的，也是学习的重点


---

## 相关阅读

- [Qt Creator 源码学习01，初识QTC](http://kevinlq.com/2021/11/01/QTC-learn01_studyQTC/)
- [Qt Creator 源码学习02，认识框架结构结构](http://kevinlq.com/2021/11/02/QTC-learn02_studyFramework/)











**相关阅读**
- Qt Creator 学习笔记01，初识 QTC[^1]
- Qt Creator 源码学习笔记02，认识框架结构^2]


[^1]: http://kevinlq.com/2021/11/01/learn01_studyQTC/
[^2]: http://kevinlq.com/2021/11/02/QTC-learn02_studyFramework/

******

    作者:鹅卵石
    时间: 2021年12月1日00:04:18
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

---
