---
layout: post
title: Qt Creator 源码学习笔记02，认识框架结构
categories: [Qt]
description: Qt Creator 源码学习笔记 02
keywords: Qt, 源码,Qt Creator
---



> 阅读本文大概需要 6 分钟

在上一篇大概了解了关于`Qt Creator` 基础概念后[^1]，本篇先学习下框架基本结构，这样能够清晰的知道那些文件和工程分别是干什么的的

## 文件结构

打开下载好的源码，如下目录所示

![](https://gitee.com/devstone/imageBed/raw/master/images/202111011414146.png)

可以看出来，文件和文件夹很多，不要被这些表面吓着，我们真正需要关心的没有几个，需要重点关注的我加粗显示了

- bin文件夹
- dist 文件夹
- doc 文件夹
- qbs 文件夹
- scripts 文件夹
- share 文件夹
- **src 文件夹**
- tests 文件夹
- docs.pri
- **qtcreator.pri**
- **qtcreator.pro**
- qtcreator.qbs
- qtcreatordata.pri
- README.md


这里我们主要要关注`src`文件夹，这个下面是这个框架的源码,其它的文件夹先不看


`qtcreator.pri`文件是项目工程中的一些通用配置，比如版本号，一些库的输出路径定义，每个插件或者子工程都会包含该配置文件，方便直接配置工程一些变量(具体怎么配置，后面会讲解到)

`qtcreator.pro`文件是主工程文件，要打开编译源码也是需要打开该工程文件进行加载的

> PS: 涉及到 qbs 相关内容可以不用关注了，Qt Build Suite 也是一种跨平台的编译工具，目前使用较少无需关注


## 框架结构

下面来详细看下工程结构是如何管理的，以及整个框架原理

![](https://gitee.com/devstone/imageBed/raw/master/images/202111011630608.png)

使用 `Qt Creator` 打开工程后你会发现有很多子工程项目，这个时候不要乱、不要怕，我们目前只需要关心三个部分就可以了


- libs
- plugins
- app

### libs部分

`libs`工程下面包含了常用的一些通用方法，我们目前关注三个即可

**`Aggregation`工程**

这个类提供了「打包」功能，可以将很多组件打包成一个整体，整个理解起来有点抽象，你可以理解为将多个对象封装成一个对象，这个对象对外提供了所有对象的接口属性和方法

- `Aggregation` 集合内部每个组件对象都可以互相转化
- `Aggregation` 集合内每个对象的生命周期被绑定在了一起，即一个在全部在，一个被删除析构那么其余的组件也就会被析构


**`extensionsystem`工程**

这个类实现了插件的管理功能，是整个框架的核心部分，所有的插件生命周期管理都在这个类里面实现

- `IPlugin` 插件基类，后面所有的插件都是继承自它来实现所有功能的，有三个重点方法需要关注

```
    virtual bool initialize(const QStringList &arguments, QString *errorString) = 0;
    virtual void extensionsInitialized() = 0;
    virtual bool delayedInitialize() { return false; }
```

插件的初始化，外部依赖初始化，延迟初始化，这三个虚函数用来初始化每个插件各自的一些资源信息。外部依赖那个也尤为重要，比如我们某个插件同时依赖多个其它插件，那么就需要在这里处理等待其它插件加载完成才算完成

- `PluginManager` 插件管理单例类，整个框架只有一份，负责框架插件的管理，随着程序退出它的声明周期才结束
- `PluginManagerPrivate` 插件管理具体实现逻辑类，看名字就很清楚，典型的`P-D`指针关系,这样是为了把插件系统扩展的具体实现隐藏不给外部暴露，这种技巧在后面很多代码中经常会见到，也是值的我们去学习
- `PluginSpec` 插件核心类，该类实现插件的所有属性

```
class EXTENSIONSYSTEM_EXPORT PluginSpec
{
public:
    enum State { Invalid, Read, Resolved, Loaded, Initialized, Running, Stopped, Deleted};

    ~PluginSpec();
    
    // 插件名字
    QString name() const;
    // 插件版本
    QString version() const;
    // 插件兼容版本
    QString compatVersion() const;

    // 插件提供者
    QString vendor() const;

    // 插件版权
    QString copyright() const;

    // 插件协议
    QString license() const;

    // 插件描述
    QString description() const;

    // 插件主页 URL
    QString url() const;
    
    // 插件类别，用于在界面分组显示插件信息
    QString category() const;
}

```

每个插件(每个动态库)都有一份该对象，用来记录该插件的所有属性信息，这些属性信息是通过 `json`配置文件读入的，这些信息被称为插件的「元信息」，后面关注插件实现会提到

**utils** 工程

这个工程里面封装了一些基础功能算法类，比如文件操作、数据排序操作、json交互操作、字符串操作集合等，还有一些基础封装控件实现也在这个里面

比如后面要提到的核心插件主窗口`QMainWindow`类，基类就在在这里

```
class QTCREATOR_UTILS_EXPORT AppMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    AppMainWindow();

public slots:
    void raiseWindow();

signals:
    void deviceChange();

#ifdef Q_OS_WIN
protected:
    virtual bool winEvent(MSG *message, long *result);
    virtual bool event(QEvent *event);
#endif

private:
    const int m_deviceEventId;
};
```
这里主要是一些事件变化后通知外部处理，比如这里如果主题发生改变发送对应信号出去，设备发生改变(插拔光驱等)发出一个设备改变事件到 `Qt` 事件队列去处理


### plugin 部分

这部分是每个插件实现部分，重点需要关注核心插件`corePlugin`的实现，其它插件都是要依赖核心插件来实现业务功能

![](https://gitee.com/devstone/imageBed/raw/master/images/202111012246718.png)

在这个插件里面主要初始化了主窗口、菜单管理类实例以及一些模式管理对象初始化

后面我们会看到各种各样的插件，比如你打开`Qt Creator`的时候首页显示的内容，也是单独的一个插件，名字叫做`weilcome`

每个插件都有一个标识`ID`，用来区分是你自己写的插件，防止别人恶意修改插件

```
Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Core.json")
Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Welcome.json")
```
每个插件还有一个对应的元数据描述配置文件，这个文件配置了该插件的一些基本信息，比如插件名字、版本、所有权、依赖那些插件等，这些配置信息在编译时会写进该插件动态库当中，采用的是`Qt`的元对象技术来实现的，这样在插件加载运行时就能通过反射动态获取这些信息，继而用来进行一些插件之间加载关系的验证

一个简单的配置描述如下所示
```
{
    \"Name\" : \"Welcome\",
    \"Version\" : \"$$QTCREATOR_VERSION\",
    \"CompatVersion\" : \"$$QTCREATOR_COMPAT_VERSION\",
    \"Vendor\" : \"The Qt Company Ltd\",
    \"Copyright\" : \"(C) $$QTCREATOR_COPYRIGHT_YEAR The Qt Company Ltd\",
    \"License\" : [ \"Commercial Usage\",
                  \"\",
                  \"Licensees holding valid Qt Commercial licenses may use this plugin in accordance with the Qt Commercial License Agreement provided with the Software or, alternatively, in accordance with the terms contained in a written agreement between you and The Qt Company.\",
                  \"\",
                  \"GNU General Public License Usage\",
                  \"\",
                  \"Alternatively, this plugin may be used under the terms of the GNU General Public License version 3 as published by the Free Software Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT included in the packaging of this plugin. Please review the following information to ensure the GNU General Public License requirements will be met: https://www.gnu.org/licenses/gpl-3.0.html.\"
    ],
    \"Category\" : \"Qt Creator\",
    \"Description\" : \"Secondary Welcome Screen Plugin.\",
    \"Url\" : \"http://www.qt.io\",
    $$dependencyList
}
```

其中很关键的是一些变量值，比如`$$QTCREATOR_VERSION`，通过这个变量直接可以读取到我们在工程`qtcreator.pri`中定义的变量值，继而快速统一加载显示，其它变量值获取类似

其次，需要关注的是每个插件的配置依赖文件比如`welcome_dependencies.pri`，该文件中包含了依赖那些库那些插件

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

某个插进依赖那些插件和动态库，只需要在对应位置追加其名字即可，工程配置文件会自动进行加载，这样编写可以减少很多重复工作，而且插件依赖关系也很清楚的看到

### app 部分

这个部分是程序入口实现部分，这里主要是获取插件路径，初始化插件、配置文件，加载每个插件，如果都没有错误，那么初始化完成后主界面就会显示出来，直接看主函数入口看就行

关键部分是插件管理器的初始化，设置插件搜索路径后对每个插件进行初始化操作

```
    PluginManager pluginManager;
    PluginManager::setPluginIID(QLatin1String("org.qt-project.Qt.QtCreatorPlugin"));
    PluginManager::setGlobalSettings(globalSettings);
    PluginManager::setSettings(settings);
    ......
    const QStringList pluginPaths = getPluginPaths() + customPluginPaths;
    PluginManager::setPluginPaths(pluginPaths);
    
    ......
    PluginManager::loadPlugins();
```

在这里还有一个需要注意的地方，就是这个文件`app_version.h.in`

这个是一个模板文件，`qmake`加载执行完毕后，会在临时目录下生成对应的头文件`app_version.h`


```
#pragma once

namespace Core {
namespace Constants {

#define STRINGIFY_INTERNAL(x) #x
#define STRINGIFY(x) STRINGIFY_INTERNAL(x)

const char IDE_DISPLAY_NAME[] = \"$${IDE_DISPLAY_NAME}\";
const char IDE_ID[] = \"$${IDE_ID}\";
const char IDE_CASED_ID[] = \"$${IDE_CASED_ID}\";

#define IDE_VERSION $${QTCREATOR_VERSION}
#define IDE_VERSION_STR STRINGIFY(IDE_VERSION)
#define IDE_VERSION_DISPLAY_DEF $${QTCREATOR_DISPLAY_VERSION}

#define IDE_VERSION_MAJOR $$replace(QTCREATOR_VERSION, "^(\\d+)\\.\\d+\\.\\d+(-.*)?$", \\1)
#define IDE_VERSION_MINOR $$replace(QTCREATOR_VERSION, "^\\d+\\.(\\d+)\\.\\d+(-.*)?$", \\1)
#define IDE_VERSION_RELEASE $$replace(QTCREATOR_VERSION, "^\\d+\\.\\d+\\.(\\d+)(-.*)?$", \\1)

const char * const IDE_VERSION_LONG      = IDE_VERSION_STR;
const char * const IDE_VERSION_DISPLAY   = STRINGIFY(IDE_VERSION_DISPLAY_DEF);
const char * const IDE_AUTHOR            = \"The Qt Company Ltd\";
const char * const IDE_YEAR              = \"$${QTCREATOR_COPYRIGHT_YEAR}\";

#ifdef IDE_REVISION
const char * const IDE_REVISION_STR      = STRINGIFY(IDE_REVISION);
#else
const char * const IDE_REVISION_STR      = \"\";
#endif
...

} // Constants
} // Core
```

这个模板文件定义了一些常量，某些变量值引用的是宏定义，最后编译后宏定义会被替换掉真正的值，在我们代码中引入时真正起作用，更加详细使用过程后面统一分析`pro`文件技巧时会提到


## 总结

学习到这里，已经大概清楚了`Qt Creator`框架的基本结构了，首先是一些基本库，这些动态库封装了一些基本功能和用法，方便在多个模块重复调用使用，其次是插件管理系统的实现，主要包含插件对象声明周期管理，插件加载、插件卸载、插件直接依赖关系处理

比如有插件A、B、C，C插件现在同时依赖于插件A和B，那么在加载时就需要特殊考虑

最后就是多个插件的初始化，主窗口和菜单组件管理类，方便拓展到其它插件进行访问管理

整个`QTC`插件系统是由一个个动态库构成的，每个插件互相配合实现了这样一个复杂的跨平台的`IDE`，仔细研究下就可以发现很多奇妙的用法和知识



**相关阅读**
- Qt Creator 学习笔记01，初识 QTC[^1]


[^1]: http://kevinlq.com/2021/11/01/learn01_studyQTC/

******

    作者:鹅卵石
    时间: 2021年11月08日23:00
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
