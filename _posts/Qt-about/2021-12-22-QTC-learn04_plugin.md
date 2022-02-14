---
layout: post
title: Qt Creator 源码学习笔记04，多插件实现原理分析
categories: [Qt]
description: Qt Creator 源码学习笔记 04
keywords: Qt, 源码,Qt Creator
---

> 阅读本文大概需要 8 分钟

插件听上去很高大上，实际上就是一个个动态库，动态库在不同平台下后缀名不一样，比如在 `Windows`下以`.dll`结尾，`Linux` 下以`.so`结尾

开发插件其实就是开发一个动态库，该动态库能够很好的加载进主程序、访问主程序资源、和主程序之间进行通信

本篇文章一起学习下 `Qt Creator`当中是如何实现插件的开发、加载、解析、管理以及互相通信的，方便我们开发自定义插件打下基础

## 简介
`Qt Creator `插件理解起来其实很简单，定义一个接口类作为基类，其他插件需要继承该类实现对应的虚方法，每个插件作为独立子工程编译后生成对应的动态库

![](https://gitee.com/devstone/imageBed/raw/master/images/202112192150398.png)

主函数加载每个插件对象，然后转化为对应插件实例

```
QPluginLoader loader(pluginName);
loader.load();
IPlugin *pluginObject = qobject_cast<IPlugin*>(loader.instance());

// 比如转为核心插件实例
CorePlugin *pCorePluginObj = qobject_cast<CorePlugin*>(loader.instance());
```

然后每个插件各自根据对应业务逻辑调用接口就行了

当然了，`Qt Creator` 在实现过程当中肯定不止这么简单，插件的加载、解析、卸载等管理还是比较复杂的，非常值得我们去学习


## 插件组成

整个插件系统由插件管理器、核心插件、其它插件组成，其中核心插件是系统中不可缺少的，其它插件都要依赖核心插件来进行开发通信

我们先打开 `Qt Creator` 插件菜单看看都包含那些插件

![](https://gitee.com/devstone/imageBed/raw/master/images/202112062325279.png)

可以看到所有的插件根据类型进行了分组，同一个类型插件同属一个树节点，每个插件后面有个复选框可以控制加载/卸载该插件

每个插件还包含了版本信息以及归属作者信息，这些信息都可以通过对象元数据来配置，插件的版本也很有用，我们编写的插件可以限定在某个版本之间兼容，这个时候版本号就起作用了，详细实现后面会讲解到

我们可以加载、卸载某个插件，但是无论怎么选择，核心`Core`插件是不能卸载的，why? 因为整个插件系统是建立在 `Core` 核心插件基础之上的，离开核心插件其它插件无法存活


所以我们学习的重点应该放在核心插件上，学会后其它插件很容易上手了

## 插件管理

插件的核心其实就是对插件的管理，这个是本篇的重点，是我们阅读源码时需要重点关注的部分，为什么这么说呢，我举个栗子大家就清楚了

我们日常写代码的时候，比如定义一个变量，需要关注的有这么几点：

- 变量的名
- 变量的值
- 变量的类型
- 变量的作用域
- 变量的生命周期

对每个定义的变量都非常清楚它的一些要素，那么肯定不会出错的

插件也一样，每个插件到实际开发当中也是一个个对象，我们定义的对象是什么类型？名字叫什么？它的值是多少？它的作用域范围是什么？生命周期呢？什么时候创建和释放？

搞清楚上述这些，对于理解插件管理工作就更进一步了，下面重点来看看插件的生命周期

### 插件管理器 

插件管理器实现主要在`PluginManager` 类当中实现，该类管理了所有的插件加载、卸载以及释放

#### 对象管理池

```
class EXTENSIONSYSTEM_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager *instance();
    static void addObject(QObject *obj);
    static void removeObject(QObject *obj);
    
    ......
    
    friend class Internal::PluginManagerPrivate;
}
```

这个类是一个单例类，主要管理插件对象，可以理解为对象池，详细实现都封装在了 `d`指针类里面，

我们继续进去看看

pluginmanager_p.h

```
class EXTENSIONSYSTEM_EXPORT PluginManagerPrivate : public QObject
{
    Q_OBJECT
public:
    ......
    QHash<QString, QList<PluginSpec *>> pluginCategories;
    QList<PluginSpec *> pluginSpecs;
    QList<QObject *> allObjects; // ### make this a QList<QPointer<QObject> > > ?
    
    ......
}
```

可以看到底层存储每个对象用的容器是 `QList`，从`Qt Creator 4.10`版本开始换成了 `QVector`来存储，说起来这两个容器的区别让我想到了，现在最新版本的 `Qt`当中，已经把两者合二为一了

```
template<typename T> using QVector = QList<T>;
```
所以使用哪个无所谓了，不过我们还是要搞清楚这两个容器的区别，什么时候用`Vector`，什么时候用 `List`

**添加对象**

```
void PluginManagerPrivate::addObject(QObject *obj)
{
    {
        QWriteLocker lock(&m_lock);
        if (obj == 0) {
            qWarning() << "PluginManagerPrivate::addObject(): trying to add null object";
            return;
        }
        if (allObjects.contains(obj)) {
            qWarning() << "PluginManagerPrivate::addObject(): trying to add duplicate object";
            return;
        }

        allObjects.append(obj);
    }
    emit q->objectAdded(obj);
}

```

这块核心代码其实很好理解，每次添加对象前先加锁，由于使用的是读写锁，不用担心函数返回死锁问题，判断对象是否合法以及是否已经存在，不存在则追加到 `list` 当中，最后抛出一个信号，这个信号在外部需要使用的地方可以绑定，比如模式切换里面就使用到了

```
void ModeManager::init()
{
    QObject::connect(ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::objectAdded,
                     m_instance, &ModeManager::objectAdded);
}

```

添加就对应的删除，原理和添加一样

**- 删除对象**

```
void PluginManagerPrivate::removeObject(QObject *obj)
{
    if (obj == 0) {
        qWarning() << "PluginManagerPrivate::removeObject(): trying to remove null object";
        return;
    }

    if (!allObjects.contains(obj)) {
        qWarning() << "PluginManagerPrivate::removeObject(): object not in list:"
            << obj << obj->objectName();
        return;
    }

    emit q->aboutToRemoveObject(obj);
    QWriteLocker lock(&m_lock);
    allObjects.removeAll(obj);
}
```

同样的把对象从`list` 当中进行了删除，在删除之前也向外抛出了信号，用法和添加信号配对使用

这里有个疑问，为啥锁不在函数最开头加呢？


#### 插件管理 

每个插件对象对应到底层是由 `PluginSpec` 来实例化的，每个插件使用 `list`容器存储，如下所示
```
QList<PluginSpec *> pluginSpecs;
```

插件核心类实现

```
class EXTENSIONSYSTEM_EXPORT PluginSpec
{
public:
    QString name() const;
    QString version() const;
    QString compatVersion() const;
    QString vendor() const;
    QString copyright() const;
    ......
    bool isRequir   ed() const;
    
    ......
    QVector<PluginDependency> dependencies() const;
    
private:
    PluginSpec();
}
```

阅读代码就可以发现，这个类主要是记录了每个插件的一些基本信息，那么这些信息是如何赋值的呢？通过插件描述文件来进行自动加载的，后面学习核心插件会看到

有个核心部分代码，插件依赖项`dependencies`，这个主要解决插件之间依赖关系使用，这个类也很简单很好理解

```
/*
 * 插件依赖相关信息
*/
struct EXTENSIONSYSTEM_EXPORT PluginDependency
{
    enum Type {
        Required,       // 必须有此依赖
        Optional,       // 此依赖不是必须的
        Test
    };

    PluginDependency() : type(Required) {}

    QString name;           //被依赖的插件名字
    QString version;        //对应的版本号
    Type type;              //依赖类型
    bool operator==(const PluginDependency &other) const;
    QString toString() const;
};
```

比如插件`A`依赖插件`B`和`C`，那么在插件`A`加载的时候对应的`list`当中就包含了`B,C`插件信息，必须等到这两个插件加载完成后才能加载插件`A`，这一点很重要

#### 插件加载流程

前面学习了插件管理器当中的一些基本数据结构，现在来看看这些插件是怎么加载进去的，加载顺序和流程是怎么样的


插件加载流程比较复杂一些，同时也是最重要的部分，主要分为下面几个步骤

![插件加载流程](https://gitee.com/devstone/imageBed/raw/master/images/202112142333136.png)

下面我们来详细看看每个步骤都干了那些工作，源码面前了无秘密


**设置插件 IID**

```    
setPluginIID(const QString &iid)
```
这个id 是全局唯一，加载插件时会首先判断插件 ID 合法性，用于确定是你自己编写的插件，这样可以防止其它插件恶意注册加载

大家可以想想一下，如果别人也写了一个类似的插件，那么如果没有 ID 区分是不是就能加载进插件系统当中，从而破坏软件结构？

`Qt Creator` 默认的 ID 为 `org.qt-project.Qt.QtCreatorPlugin`，每个插件加载时通过宏进行设置

```
class CorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Core.json")
}

宏展开后：

#define Q_PLUGIN_METADATA(x) QT_ANNOTATE_CLASS(qt_plugin_metadata, x)

#define QT_ANNOTATE_CLASS(type, ...)
```

这个宏是为了配合`moc`处理器生成插件导出函数，最终在调用插件接口返回实例时能够准确返回自己。我们写个 `demo` 来验证下

新建一个插件叫做 `PluginDemo`

```
class PluginDemo : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "PluginDemo.json")
};
```

qmake 编译一下看下中间结果内容：
```
static const qt_meta_stringdata_PluginDemo_t qt_meta_stringdata_PluginDemo = {
    {
QT_MOC_LITERAL(0, 0, 10) // "PluginDemo"

    },
    "PluginDemo"
};
void *PluginDemo::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PluginDemo.stringdata0))
        return static_cast<void*>(const_cast< PluginDemo*>(this));
    return ExtensionSystem::IPlugin::qt_metacast(_clname);
}
```


**设置全局配置类**
```
setGlobalSettings(QSettings *settings)
```
> 全局配置，一般存放的是默认值,用于恢复设置使用

**设置局部配置类**
```
setSettings(QSettings *settings)
```

> 存放程序当前配置参数类。比如我们设置某个参数配置保存后会存在某个配置文件中，程序加载时会从该文件加载到`QSettings`对象当中供我们调用


**设置插件路径**
```
setPluginPaths(const QStringList &paths)
```

> 插件路径一般是我们 exe 程序相邻路径下的，比如plugins/xxx.dll，当然也可以为任意路径下的动态库，只要路径正确合法都可以加载的，可以设置多条插件路径

比如正常 `Qt Creator`启动时会给两个路径分别为：

```
 ("D:/CloundCode/QTC/bin/Win32/Debug/QTC/lib/qtc/plugins",
 "C:/Users/devstone/AppData/Local/QtProject/qtc/plugins/4.4.1")
```

关于路径的获取可以看后面主程序加载部分可以看到

**读取插件信息**

> 用于读取插件原对象信息，主要包含三个过程

```
readMetaData()
resolveDependencies()
pluginsChanged()
```
- 读元数据：这里会挨个读取每个插件，初始化 QPluginLoader，设置名字，为后面加载做准备，可以叫预加载，创建插件实例对象 PluginSpec，存储到 List 结构当中
- 检测依赖关系：：用于重新加载分析每个插件依赖关系，是一个双重循环，每个插件会和其它插件比较一次，最后按照插件名字进行排序
- 插件改变：向外抛出信号，插件管理窗口用来刷新 view 列表信息


**加载插件**

> 到了这里才开始真正加载插件了，主要包括下面几个流程

```
loadQueue（）
loadPlugins()
(PluginSpec::Loaded)
(PluginSpec::Initialized)
(PluginSpec::Running)
```

- 依赖初始化
- 加载插件:这里里面才会真真去加载初始化每个插件,计算获取插件加载队列
- 加载(PluginSpec::Loaded):

```
loadPlugin(PluginSpec *spec, PluginSpec::State destState)
```
调用 QPluginLoader.load()，真正加载插件，加载成功才可以获取每个插件方法,存储插件实例：
```
IPlugin *pluginObject = qobject_cast<IPlugin*>(loader.instance());
```

- 初始化(PluginSpec::Initialized)
```
loadPlugin(PluginSpec *spec, PluginSpec::State destState)
```
这里会调用每个插件的初始化函数：initialize(),该函数是纯虚函数，每个插件必须重新实现

- 运行(PluginSpec::Running)
```
loadPlugin(PluginSpec *spec, PluginSpec::State destState)
```
调用每个插件扩展初始化函数：extensionsInitialized（）,此时会挨个判断买个插件状态是否在运行，是的话加入到延迟队列

- 延迟初始化

```
nextDelayedInitialize（）
```
> 从延迟队列当中取出买个插件，调用各自延迟初始化函数：delayedInitialize（）


**插件加载结束**

到此整个插件加载结束了，可以看出来，整个插件的加载过程说白了就是动态库加载解析然后调用每个动态库里面的虚函数来实现的，所有的插件都继承自共同的基类(接口)，原理很简单，但是要管理这些插件尤其是多种依赖关系并存情况下是非常不容易的

看到这里大家是不是很好奇，为啥不引用头文件直接可以调用动态库里面的方法了？这个主要使用 `QPluginLoader` 来实现动态加载动态库，这个类很好理解，详细使用可以看我之前写的`SDK`调用相关文章

包含了使用示例以及对应解析

```
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

[如何使用 QLibrary 加载动态库](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484281&idx=1&sn=9fa222c1b2f5e0f0d5c3f7cb230719ad&chksm=e85c0d49df2b845f7f59827b3758b5dfa3973679d6b7f5772f8e78d000fa92d4a156465ec4bb&token=201364907&lang=zh_CN#rd)



## 核心插件

学习了解清楚了插件如何管理，如何加载，下面来看看核心插件如何实现，以及如何实现自己的插件

### 插件描述文件

插件描述文件一般用于记录每个插件的基本信息，必须有，而且字段和用法都是固定的。名字一般取插件名字，结尾一般都是`.json.in`

> 看到这里是不是好奇，我记得自己第一次看到时也很好奇，为啥是`.in`结尾，这个其实是一个模板文件，经过`qmake`构建后最终在临时目录下会生成最终的去掉`.in`的文件

Core.json.in


插件代码中包含该文件

```
class CorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Core.json")
};
```
文件内容大概如下所示：

``` JSON
{
    \"Name\" : \"Core\",
    \"Version\" : \"$$QTCREATOR_VERSION\",
    \"CompatVersion\" : \"$$QTCREATOR_COMPAT_VERSION\",
    \"Required\" : true,
    \"HiddenByDefault\" : true,
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
    \"Description\" : \"The core plugin for the Qt IDE.\",
    \"Url\" : \"http://www.qt.io\",
    \"Arguments\" : [
        {
            \"Name\" : \"-color\",
            \"Parameter\" : \"color\",
            \"Description\" : \"Override selected UI color\"
        },
        {
            \"Name\" : \"-theme\",
            \"Parameter\" : \"default|dark\",
            \"Description\" : \"Choose a built-in theme or pass a .creatortheme file\"
        },
        {
            \"Name\" : \"-presentationMode\",
            \"Description\" : \"Enable presentation mode with pop-ups for key combos\"
        }
    ],
    $$dependencyList
}
```

其实就是一个标准的`json`配置文件，每个字段都很清楚，可能有些变量值不清楚，这里一起学习下。比如版本号字段：

```
\"Version\" : \"$$QTCREATOR_VERSION\",
```

很明显后面是一个变量值，也可以说是宏定义，我们一般定义`json`配置都是固定值，这里采用动态配置方法，其中`QTCREATOR_VERSION` 变量是在`pro`工程中定义的

这样做有什么好处呢？想一想

是不是我们下次变更版本号的时候，直接在`pro`文件中更改一次，其它引用到该变量的地方都自动同步了，是不是很方便而且可以减少很多出错(这就是软件开发当中的封装思想)

其实，除过在配置文件中可以引用变量以外，在代码中也可以直接引用，关于如何使用，可以看我之前写的一篇文章，详细介绍了原理以及实现方法

[qmake奇淫技巧之字符串宏定义](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484160&idx=1&sn=f9a8d86d093258168f7099726270a75d&chksm=e85c0d30df2b8426b9732c8399312da789e8644a14d01d105653c87fbbc539c7cdc4326ced42&token=201364907&lang=zh_CN#rd)

### 核心插件初始化
核心插件主要初始化基本界面结构，包含一个`QMainWindow`、菜单栏、状态栏、模式工具栏、多窗口面板等等

正如第一篇笔记当中写到，如果只编译核心插件，那么运行后整个界面张这个样子

![](https://gitee.com/devstone/imageBed/raw/master/images/202112192158402.png)

可以看到仅仅包含基本菜单，插件查看，状态栏等内容

每个插件都需要实现自己的初始化函数

```
bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    ......
}
```
初始化函数当中首先要注册所有插件的`mime type`类型，这个是从插件元数据当中读取的，会跳过已经关闭的插件

接着初始化系统主题，主题其实和我们经常用的 `qss` 样式表类似，大概张这个样子

```
[General]
ThemeName=Flat Dark
PreferredStyles=Fusion
DefaultTextEditorColorScheme=creator-dark.xml

[Palette]
shadowBackground=ff404244
text=ffd0d0d0
textDisabled=60a4a6a8
toolBarItem=b6fbfdff
```

其实就是一个`.ini`文件格式的内容，定义了很多界面样式相关变量字段，这些字段会一一映射到对应主题管理类当中，这样相关界面设置样式就可以直接调用了

接着也是一个很重要的模块，初始化菜单管理类，这个类管理了菜单栏所有的菜单/Action，以供其它插件模块访问

```
    new ActionManager(this);
    ActionManager::setPresentationModeEnabled(args.presentationMode);
```

`ActionManager` 这个类是一个特殊的单例类，单例对象初始化只能在核心插件当中，虽然提供了单例返回接口，但是首次如果没有初始化对象返回的是`空指针`

```
class CORE_EXPORT ActionManager : public QObject
{
    Q_OBJECT
public:
    static ActionManager *instance();
private:
    ActionManager(QObject *parent = 0);
    ~ActionManager();

    friend class Core::Internal::CorePlugin; // initialization
    friend class Core::Internal::MainWindow; // saving settings and setting context
};

static ActionManager *m_instance = 0;
ActionManager *ActionManager::instance()
{
    return m_instance;
}
```

所有才有了后面两个友元类的声明了，这样可以直接访问并且初始化对象实例了，核心插件初始化完成后，其它地方可以直接调用单例函数了


接着就是主界面初始化，初始化 `mainWindow`实例
```
    m_mainWindow = new MainWindow;
    if (args.overrideColor.isValid())
        m_mainWindow->setOverrideColor(args.overrideColor);
    qsrand(QDateTime::currentDateTime().toTime_t());
    const bool success = m_mainWindow->init(errorMessage);
```

主界面实例初始化后，接着会调用主界面的初始化函数，主界面真正初始化了多插件界面实现，如果想要学习多插件界面是如何实现的，可以重点关注下这个初始化函数

最后是编辑模式、查找相关功能初始化，这些功能不是本次重点，后面有需要再详细看实现思想

### 主界面初始化

主界面和我们平时创建项目使用的`QMainWindow`没有两样，最大的区别就是`Qt Creator`把界面上所有的操作都进行了封装管理，这样其它插件也可以进行访问，更好的对界面系统进行了扩展

主界面我们重点来学习了菜单栏的使用，看看是如何封装管理的

主要涉及到下面几个类

- ActionContainer
- ActionContainerPrivate
- MenuActionContainer
- MenuBarActionContainer
- ActionManager

这些类的关系如下所示

![](https://gitee.com/devstone/imageBed/raw/master/images/202112192256200.png)

其中 `ActionContainer`对象是基类，向外部所有插件暴露，后面访问某个菜单大部分场景是返回该类指针的

`MenuActionContainer` 是菜单栏当中的菜单对象，可以包含 `n` 个菜单

`MenuBarActionContainer` 是我们的菜单栏，整个 `MainWindows`仅此一份实例

最后就是 `ActionManager`类了，我们所有的操作均是通过该类来进行，很显然它是一个单例类，而且整个系统都是可以访问的

- 创建菜单栏

```C++
ActionContainer *menubar = ActionManager::createMenuBar(Constants::MENU_BAR);

if (!HostOsInfo::isMacHost()) // System menu bar on Mac
    setMenuBar(menubar->menuBar());
```

- 创建文件菜单

```
    ActionContainer *filemenu = ActionManager::createMenu(Constants::M_FILE);
    menubar->addMenu(filemenu, Constants::G_FILE);
    filemenu->menu()->setTitle(tr("&File"));
```

可以看到使用是来是非常方便的，而且这种通过传入字符串创建菜单的方式也简单理解，外部使用的人员完全不用了解函数内部是怎么实现的，只需要根据自己需要传入规定格式的字符串即可

每个菜单都有唯一的字符串 `ID`来进行区分，字符串命名严格按照菜单格式，比如

```
const char M_FILE[]                = "QtCreator.Menu.File";
const char M_FILE_RECENTFILES[]    = "QtCreator.Menu.File.RecentFiles";
const char M_EDIT[]                = "QtCreator.Menu.Edit";
```

这样的格式也很好理解，`Menu`相当于是大菜单，后面一级是每个子菜单，如果该菜单还有子菜单，那么继续扩展下去

其它界面菜单栏菜单创建和上面的过程是类似的，可以照猫画虎写出来

- 创建每个 Action

上面创建了界面的菜单栏，但是每个菜单下面还是空的，需要创建对应的 `Action` 才行，下面来看看是怎么创建的

```C++
    ActionContainer *mfile = ActionManager::actionContainer(Constants::M_FILE);
    
    mfile->addSeparator(Constants::G_FILE_SAVE);

    // Open Action
    QIcon icon = QIcon::fromTheme(QLatin1String("document-open"), Utils::Icons::OPENFILE.icon());
    QAction *m_openAction = new QAction(icon, tr("&Open File or Project..."), this);
    Command *cmd = ActionManager::registerAction(m_openAction, Constants::OPEN);
    cmd->setDefaultKeySequence(QKeySequence::Open);
    mfile->addAction(cmd, Constants::G_FILE_OPEN);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);
```

第一行代码通过菜单管理器返回上面创建的「文件」菜单指针，第二行添加了一个分隔符，后面创建了一个`Command`对象，这个类是对每个`QAction`进行了封装，同时支持设置快捷键等操作，这样我们后续的操作就相当于是一个`command`

这样我们的菜单栏就创建初始化完成了，剩下的就是左侧模式工具条以及中央内容区域的创建了

限于篇幅原因，这些内容我们后面在看

## App 程序初始化

前面花费了大量篇幅来介绍插件的管理以及主界面的实现，下面我们来看看主程序是如何初始化的

主函数 `main.cpp` 里面的内容挺多的，我们看主要加载流程就行了

- 设置系统配置对象指针

```
    QSettings *settings = userSettings();
    QSettings *globalSettings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,
                                              QLatin1String(Core::Constants::IDE_SETTINGSVARIANT_STR),
                                              QLatin1String("QTC"));
    PluginManager pluginManager;
    PluginManager::setPluginIID(QLatin1String("org.qt-project.Qt.QtCreatorPlugin"));
    PluginManager::setGlobalSettings(globalSettings);
    PluginManager::setSettings(settings);
```

主要是系统当中的一些配置，插件管理器需要记录那些插件被禁用了，这样在后面插件初始化时可以跳过了

其中很重要的设置插件`ID`，这个作为插件唯一标识符，用来区分恶意插件，如果别人不知道你的软件插件`IID`，那么他编写的插件放入你程序目录下是可以直接通过这个`IID`过滤掉的


- 设置插件路径

```C++
    const QStringList pluginPaths = getPluginPaths() + customPluginPaths;
    PluginManager::setPluginPaths(pluginPaths);
```

这里的插件路径包含了两部分，一部分是我们程序目录下的插件目录，另一个是公共目录,比如下面这个

```
("D:/CloundCode/QTC/bin/Win32/Debug/QTC/lib/qtc/plugins", "C:/Users/devstone/AppData/Local/QtProject/qtc/plugins/4.4.1")
```

这一步走完后，如果没有错误整个插件都加载完成了

- 异常判断

想法是美好的，但是事实总不如愿，插件在加载过程中可能会遇到一些问题导致加载异常，这样程序就无法正常运行了，需要抛出错误给用户

```C++
    const PluginSpecSet plugins = PluginManager::plugins();
    PluginSpec *coreplugin = 0;
    foreach (PluginSpec *spec, plugins) {
        if (spec->name() == QLatin1String(corePluginNameC)) {
            coreplugin = spec;
            break;
        }
    }
    if (!coreplugin) {
        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1Char(',')));
        const QString reason = QCoreApplication::translate("Application", "Could not find Core plugin in %1").arg(nativePaths);
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (!coreplugin->isEffectivelyEnabled()) {
        const QString reason = QCoreApplication::translate("Application", "Core plugin is disabled.");
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }
```

这段代码对核插件加载状况进行了判断，如果有错误没有加载完成或者被禁用了，那么就直接返回了。理论上来说核心插件是无法被禁用的，但是如果有人恶意修改配置文件禁用了核心插件，那么此时程序会无法正常启动的


- 加载插件

这一步其实是最重要的，上面设置插件路径后仅仅是读取每个插件对象，此时这些对应都是静态的，到了这一步才真正动起来

```C++
PluginManager::loadPlugins();
if (coreplugin->hasError()) {
    displayError(msgCoreLoadFailure(coreplugin->errorString()));
    return 1;
}
```

关于插件加载这个流程最前面插件管理器当中介绍清楚了，这里我们直接略过就行了

好了关于插件加载学习就到这里了


## 总结

插件部分内容还是挺长，初次学习源码的朋友可能会感觉到无从下手、一脸茫然，不用担心，我第一次也是这种感觉，遇到不懂不理解的小标记下，先理解掌握整体设计思想和流程，再慢慢逐个模块攻破

软件开发也是这个道理，一开始你不可能考虑到所有模块细节，把握整体结构没有问题，再挨个实现细节部分

`Qt Creator`非常值得我们多看、多写的，所谓好事多磨么，看的多了也就明白了一些道理

我们日常开发过程中遇到的一些问题，可能`Qt Creator`当中早就实现好了，可以直接拿来使用，比如像奔溃`dump`管理、日志管理、网络通信、特殊控件实现等都可以拿来直接用的

希望本次分享的笔记对你有帮助，如果觉得有用不妨关注下，有任何问题可以互相交流学习



## 相关阅读

- Qt Creator 学习笔记01，初识 QTC[^1]
- Qt Creator 源码学习笔记02，认识框架结构[^2]
- Qt Creator 源码学习笔记03，大型项目如何管理工程[^3]
- Qt Creator 源码学习笔记04，多插件实现原理分析[^4]


[^1]: https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484658&idx=1&sn=55af25cd6e608fa9cb1452610928e71b&chksm=e85c0ac2df2b83d453d80f66fcdefca31e998a8ac0ba0226edbd80b437f99ed5184478805d8e&token=2125642557&lang=zh_CN#rd
[^2]: https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484676&idx=1&sn=c1bd8cbd78d8e098c22353f567cc3620&chksm=e85c0b34df2b822288a7baa016f4a64df47e5e119039836ff686cb2ad3a7e28a5808efc0c13a&token=2125642557&lang=zh_CN#rd
[^3]:https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484701&idx=1&sn=c69eb1edfb551c41690938423ca2ef7c&chksm=e85c0b2ddf2b823b1179f216e57ca91b9ce4068a0469e8ba062ab3596e9dc51ac05a1572da85&token=2125642557&lang=zh_CN#rd
[^4]:https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484767&idx=1&sn=971c14682095a8a84a8161311400100f&chksm=e85c0b6fdf2b82799db871eafb6d0b2465c75d8020b1f87a1f5825f82edcad2c051b7d6e1c2c&token=2125642557&lang=zh_CN#rd

******

    作者:鹅卵石
    时间: 2021年12月22日23:24:15
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
=======
---
layout: post
title: Qt Creator 源码学习笔记04，多插件实现原理分析
categories: [Qt]
description: Qt Creator 源码学习笔记 04
keywords: Qt, 源码,Qt Creator
---


> 阅读本文大概需要 8 分钟


插件听上去很高大上，实际上就是一个个动态库，动态库在不同平台下后缀名不一样，比如在 `Windows`下以`.dll`结尾，`Linux` 下以`.so`结尾

开发插件其实就是开发一个动态库，该动态库能够很好的加载进主程序、访问主程序资源、和主程序之间进行通信

本篇文章一起学习下 `Qt Creator`当中是如何实现插件的开发、加载、解析、管理以及互相通信的，方便我们开发自定义插件打下基础


## 简介
`Qt Creator `插件理解起来其实很简单，定义一个接口类作为基类，其他插件需要继承该类实现对应的虚方法，每个插件作为独立子工程编译后生成对应的动态库

![](https://gitee.com/devstone/imageBed/raw/master/images/202112192150398.png)

主函数加载每个插件对象，然后转化为对应插件实例

```
QPluginLoader loader(pluginName);
loader.load();
IPlugin *pluginObject = qobject_cast<IPlugin*>(loader.instance());

// 比如转为核心插件实例
CorePlugin *pCorePluginObj = qobject_cast<CorePlugin*>(loader.instance());
```

然后每个插件各自根据对应业务逻辑调用接口就行了

当然了，`Qt Creator` 在实现过程当中肯定不止这么简单，插件的加载、解析、卸载等管理还是比较复杂的，非常值得我们去学习


## 插件组成

整个插件系统由插件管理器、核心插件、其它插件组成，其中核心插件是系统中不可缺少的，其它插件都要依赖核心插件来进行开发通信

我们先打开 `Qt Creator` 插件菜单看看都包含那些插件

![](https://gitee.com/devstone/imageBed/raw/master/images/202112062325279.png)

可以看到所有的插件根据类型进行了分组，同一个类型插件同属一个树节点，每个插件后面有个复选框可以控制加载/卸载该插件

每个插件还包含了版本信息以及归属作者信息，这些信息都可以通过对象元数据来配置，插件的版本也很有用，我们编写的插件可以限定在某个版本之间兼容，这个时候版本号就起作用了，详细实现后面会讲解到

我们可以加载、卸载某个插件，但是无论怎么选择，核心`Core`插件是不能卸载的，why? 因为整个插件系统是建立在 `Core` 核心插件基础之上的，离开核心插件其它插件无法存活


所以我们学习的重点应该放在核心插件上，学会后其它插件很容易上手了

## 插件管理

插件的核心其实就是对插件的管理，这个是本篇的重点，是我们阅读源码时需要重点关注的部分，为什么这么说呢，我举个栗子大家就清楚了

我们日常写代码的时候，比如定义一个变量，需要关注的有这么几点：

- 变量的名
- 变量的值
- 变量的类型
- 变量的作用域
- 变量的生命周期

对每个定义的变量都非常清楚它的一些要素，那么肯定不会出错的

插件也一样，每个插件到实际开发当中也是一个个对象，我们定义的对象是什么类型？名字叫什么？它的值是多少？它的作用域范围是什么？生命周期呢？什么时候创建和释放？

搞清楚上述这些，对于理解插件管理工作就更进一步了，下面重点来看看插件的生命周期

### 插件管理器 

插件管理器实现主要在`PluginManager` 类当中实现，该类管理了所有的插件加载、卸载以及释放

#### 对象管理池

```
class EXTENSIONSYSTEM_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager *instance();
    static void addObject(QObject *obj);
    static void removeObject(QObject *obj);
    
    ......
    
    friend class Internal::PluginManagerPrivate;
}
```

这个类是一个单例类，主要管理插件对象，可以理解为对象池，详细实现都封装在了 `d`指针类里面，

我们继续进去看看

pluginmanager_p.h

```
class EXTENSIONSYSTEM_EXPORT PluginManagerPrivate : public QObject
{
    Q_OBJECT
public:
    ......
    QHash<QString, QList<PluginSpec *>> pluginCategories;
    QList<PluginSpec *> pluginSpecs;
    QList<QObject *> allObjects; // ### make this a QList<QPointer<QObject> > > ?
    
    ......
}
```

可以看到底层存储每个对象用的容器是 `QList`，从`Qt Creator 4.10`版本开始换成了 `QVector`来存储，说起来这两个容器的区别让我想到了，现在最新版本的 `Qt`当中，已经把两者合二为一了

```
template<typename T> using QVector = QList<T>;
```
所以使用哪个无所谓了，不过我们还是要搞清楚这两个容器的区别，什么时候用`Vector`，什么时候用 `List`

**添加对象**

```
void PluginManagerPrivate::addObject(QObject *obj)
{
    {
        QWriteLocker lock(&m_lock);
        if (obj == 0) {
            qWarning() << "PluginManagerPrivate::addObject(): trying to add null object";
            return;
        }
        if (allObjects.contains(obj)) {
            qWarning() << "PluginManagerPrivate::addObject(): trying to add duplicate object";
            return;
        }

        allObjects.append(obj);
    }
    emit q->objectAdded(obj);
}

```

这块核心代码其实很好理解，每次添加对象前先加锁，由于使用的是读写锁，不用担心函数返回死锁问题，判断对象是否合法以及是否已经存在，不存在则追加到 `list` 当中，最后抛出一个信号，这个信号在外部需要使用的地方可以绑定，比如模式切换里面就使用到了

```
void ModeManager::init()
{
    QObject::connect(ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::objectAdded,
                     m_instance, &ModeManager::objectAdded);
}

```

添加就对应的删除，原理和添加一样

**- 删除对象**

```
void PluginManagerPrivate::removeObject(QObject *obj)
{
    if (obj == 0) {
        qWarning() << "PluginManagerPrivate::removeObject(): trying to remove null object";
        return;
    }

    if (!allObjects.contains(obj)) {
        qWarning() << "PluginManagerPrivate::removeObject(): object not in list:"
            << obj << obj->objectName();
        return;
    }

    emit q->aboutToRemoveObject(obj);
    QWriteLocker lock(&m_lock);
    allObjects.removeAll(obj);
}
```

同样的把对象从`list` 当中进行了删除，在删除之前也向外抛出了信号，用法和添加信号配对使用

这里有个疑问，为啥锁不在函数最开头加呢？


#### 插件管理 

每个插件对象对应到底层是由 `PluginSpec` 来实例化的，每个插件使用 `list`容器存储，如下所示
```
QList<PluginSpec *> pluginSpecs;
```

插件核心类实现

```
class EXTENSIONSYSTEM_EXPORT PluginSpec
{
public:
    QString name() const;
    QString version() const;
    QString compatVersion() const;
    QString vendor() const;
    QString copyright() const;
    ......
    bool isRequir   ed() const;
    
    ......
    QVector<PluginDependency> dependencies() const;
    
private:
    PluginSpec();
}
```

阅读代码就可以发现，这个类主要是记录了每个插件的一些基本信息，那么这些信息是如何赋值的呢？通过插件描述文件来进行自动加载的，后面学习核心插件会看到

有个核心部分代码，插件依赖项`dependencies`，这个主要解决插件之间依赖关系使用，这个类也很简单很好理解

```
/*
 * 插件依赖相关信息
*/
struct EXTENSIONSYSTEM_EXPORT PluginDependency
{
    enum Type {
        Required,       // 必须有此依赖
        Optional,       // 此依赖不是必须的
        Test
    };

    PluginDependency() : type(Required) {}

    QString name;           //被依赖的插件名字
    QString version;        //对应的版本号
    Type type;              //依赖类型
    bool operator==(const PluginDependency &other) const;
    QString toString() const;
};
```

比如插件`A`依赖插件`B`和`C`，那么在插件`A`加载的时候对应的`list`当中就包含了`B,C`插件信息，必须等到这两个插件加载完成后才能加载插件`A`，这一点很重要

#### 插件加载流程

前面学习了插件管理器当中的一些基本数据结构，现在来看看这些插件是怎么加载进去的，加载顺序和流程是怎么样的


插件加载流程比较复杂一些，同时也是最重要的部分，主要分为下面几个步骤

![插件加载流程](https://gitee.com/devstone/imageBed/raw/master/images/202112142333136.png)

下面我们来详细看看每个步骤都干了那些工作，源码面前了无秘密


**设置插件 IID**

```    
setPluginIID(const QString &iid)
```
这个id 是全局唯一，加载插件时会首先判断插件 ID 合法性，用于确定是你自己编写的插件，这样可以防止其它插件恶意注册加载

大家可以想想一下，如果别人也写了一个类似的插件，那么如果没有 ID 区分是不是就能加载进插件系统当中，从而破坏软件结构？

`Qt Creator` 默认的 ID 为 `org.qt-project.Qt.QtCreatorPlugin`，每个插件加载时通过宏进行设置

```
class CorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Core.json")
}

宏展开后：

#define Q_PLUGIN_METADATA(x) QT_ANNOTATE_CLASS(qt_plugin_metadata, x)

#define QT_ANNOTATE_CLASS(type, ...)
```

这个宏是为了配合`moc`处理器生成插件导出函数，最终在调用插件接口返回实例时能够准确返回自己。我们写个 `demo` 来验证下

新建一个插件叫做 `PluginDemo`

```
class PluginDemo : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "PluginDemo.json")
};
```

qmake 编译一下看下中间结果内容：
```
static const qt_meta_stringdata_PluginDemo_t qt_meta_stringdata_PluginDemo = {
    {
QT_MOC_LITERAL(0, 0, 10) // "PluginDemo"

    },
    "PluginDemo"
};
void *PluginDemo::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PluginDemo.stringdata0))
        return static_cast<void*>(const_cast< PluginDemo*>(this));
    return ExtensionSystem::IPlugin::qt_metacast(_clname);
}
```


**设置全局配置类**
```
setGlobalSettings(QSettings *settings)
```
> 全局配置，一般存放的是默认值,用于恢复设置使用

**设置局部配置类**
```
setSettings(QSettings *settings)
```

> 存放程序当前配置参数类。比如我们设置某个参数配置保存后会存在某个配置文件中，程序加载时会从该文件加载到`QSettings`对象当中供我们调用


**设置插件路径**
```
setPluginPaths(const QStringList &paths)
```

> 插件路径一般是我们 exe 程序相邻路径下的，比如plugins/xxx.dll，当然也可以为任意路径下的动态库，只要路径正确合法都可以加载的，可以设置多条插件路径

比如正常 `Qt Creator`启动时会给两个路径分别为：

```
 ("D:/CloundCode/QTC/bin/Win32/Debug/QTC/lib/qtc/plugins",
 "C:/Users/devstone/AppData/Local/QtProject/qtc/plugins/4.4.1")
```

关于路径的获取可以看后面主程序加载部分可以看到

**读取插件信息**

> 用于读取插件原对象信息，主要包含三个过程

```
readMetaData()
resolveDependencies()
pluginsChanged()
```
- 读元数据：这里会挨个读取每个插件，初始化 QPluginLoader，设置名字，为后面加载做准备，可以叫预加载，创建插件实例对象 PluginSpec，存储到 List 结构当中
- 检测依赖关系：：用于重新加载分析每个插件依赖关系，是一个双重循环，每个插件会和其它插件比较一次，最后按照插件名字进行排序
- 插件改变：向外抛出信号，插件管理窗口用来刷新 view 列表信息


**加载插件**

> 到了这里才开始真正加载插件了，主要包括下面几个流程

```
loadQueue（）
loadPlugins()
(PluginSpec::Loaded)
(PluginSpec::Initialized)
(PluginSpec::Running)
```

- 依赖初始化
- 加载插件:这里里面才会真真去加载初始化每个插件,计算获取插件加载队列
- 加载(PluginSpec::Loaded):

```
loadPlugin(PluginSpec *spec, PluginSpec::State destState)
```
调用 QPluginLoader.load()，真正加载插件，加载成功才可以获取每个插件方法,存储插件实例：
```
IPlugin *pluginObject = qobject_cast<IPlugin*>(loader.instance());
```

- 初始化(PluginSpec::Initialized)
```
loadPlugin(PluginSpec *spec, PluginSpec::State destState)
```
这里会调用每个插件的初始化函数：initialize(),该函数是纯虚函数，每个插件必须重新实现

- 运行(PluginSpec::Running)
```
loadPlugin(PluginSpec *spec, PluginSpec::State destState)
```
调用每个插件扩展初始化函数：extensionsInitialized（）,此时会挨个判断买个插件状态是否在运行，是的话加入到延迟队列

- 延迟初始化

```
nextDelayedInitialize（）
```
> 从延迟队列当中取出买个插件，调用各自延迟初始化函数：delayedInitialize（）


**插件加载结束**

到此整个插件加载结束了，可以看出来，整个插件的加载过程说白了就是动态库加载解析然后调用每个动态库里面的虚函数来实现的，所有的插件都继承自共同的基类(接口)，原理很简单，但是要管理这些插件尤其是多种依赖关系并存情况下是非常不容易的

看到这里大家是不是很好奇，为啥不引用头文件直接可以调用动态库里面的方法了？这个主要使用 `QPluginLoader` 来实现动态加载动态库，这个类很好理解，详细使用可以看我之前写的`SDK`调用相关文章

包含了使用示例以及对应解析

```
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

[如何使用 QLibrary 加载动态库](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484281&idx=1&sn=9fa222c1b2f5e0f0d5c3f7cb230719ad&chksm=e85c0d49df2b845f7f59827b3758b5dfa3973679d6b7f5772f8e78d000fa92d4a156465ec4bb&token=201364907&lang=zh_CN#rd)



## 核心插件

学习了解清楚了插件如何管理，如何加载，下面来看看核心插件如何实现，以及如何实现自己的插件

### 插件描述文件

插件描述文件一般用于记录每个插件的基本信息，必须有，而且字段和用法都是固定的。名字一般取插件名字，结尾一般都是`.json.in`

> 看到这里是不是好奇，我记得自己第一次看到时也很好奇，为啥是`.in`结尾，这个其实是一个模板文件，经过`qmake`构建后最终在临时目录下会生成最终的去掉`.in`的文件

Core.json.in


插件代码中包含该文件

```
class CorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Core.json")
};
```
文件内容大概如下所示：

``` JSON
{
    \"Name\" : \"Core\",
    \"Version\" : \"$$QTCREATOR_VERSION\",
    \"CompatVersion\" : \"$$QTCREATOR_COMPAT_VERSION\",
    \"Required\" : true,
    \"HiddenByDefault\" : true,
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
    \"Description\" : \"The core plugin for the Qt IDE.\",
    \"Url\" : \"http://www.qt.io\",
    \"Arguments\" : [
        {
            \"Name\" : \"-color\",
            \"Parameter\" : \"color\",
            \"Description\" : \"Override selected UI color\"
        },
        {
            \"Name\" : \"-theme\",
            \"Parameter\" : \"default|dark\",
            \"Description\" : \"Choose a built-in theme or pass a .creatortheme file\"
        },
        {
            \"Name\" : \"-presentationMode\",
            \"Description\" : \"Enable presentation mode with pop-ups for key combos\"
        }
    ],
    $$dependencyList
}
```

其实就是一个标准的`json`配置文件，每个字段都很清楚，可能有些变量值不清楚，这里一起学习下。比如版本号字段：

```
\"Version\" : \"$$QTCREATOR_VERSION\",
```

很明显后面是一个变量值，也可以说是宏定义，我们一般定义`json`配置都是固定值，这里采用动态配置方法，其中`QTCREATOR_VERSION` 变量是在`pro`工程中定义的

这样做有什么好处呢？想一想

是不是我们下次变更版本号的时候，直接在`pro`文件中更改一次，其它引用到该变量的地方都自动同步了，是不是很方便而且可以减少很多出错(这就是软件开发当中的封装思想)

其实，除过在配置文件中可以引用变量以外，在代码中也可以直接引用，关于如何使用，可以看我之前写的一篇文章，详细介绍了原理以及实现方法

[qmake奇淫技巧之字符串宏定义](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484160&idx=1&sn=f9a8d86d093258168f7099726270a75d&chksm=e85c0d30df2b8426b9732c8399312da789e8644a14d01d105653c87fbbc539c7cdc4326ced42&token=201364907&lang=zh_CN#rd)

### 核心插件初始化
核心插件主要初始化基本界面结构，包含一个`QMainWindow`、菜单栏、状态栏、模式工具栏、多窗口面板等等

正如第一篇笔记当中写到，如果只编译核心插件，那么运行后整个界面张这个样子

![](https://gitee.com/devstone/imageBed/raw/master/images/202112192158402.png)

可以看到仅仅包含基本菜单，插件查看，状态栏等内容

每个插件都需要实现自己的初始化函数

```
bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    ......
}
```
初始化函数当中首先要注册所有插件的`mime type`类型，这个是从插件元数据当中读取的，会跳过已经关闭的插件

接着初始化系统主题，主题其实和我们经常用的 `qss` 样式表类似，大概张这个样子

```
[General]
ThemeName=Flat Dark
PreferredStyles=Fusion
DefaultTextEditorColorScheme=creator-dark.xml

[Palette]
shadowBackground=ff404244
text=ffd0d0d0
textDisabled=60a4a6a8
toolBarItem=b6fbfdff
```

其实就是一个`.ini`文件格式的内容，定义了很多界面样式相关变量字段，这些字段会一一映射到对应主题管理类当中，这样相关界面设置样式就可以直接调用了

接着也是一个很重要的模块，初始化菜单管理类，这个类管理了菜单栏所有的菜单/Action，以供其它插件模块访问

```
    new ActionManager(this);
    ActionManager::setPresentationModeEnabled(args.presentationMode);
```

`ActionManager` 这个类是一个特殊的单例类，单例对象初始化只能在核心插件当中，虽然提供了单例返回接口，但是首次如果没有初始化对象返回的是`空指针`

```
class CORE_EXPORT ActionManager : public QObject
{
    Q_OBJECT
public:
    static ActionManager *instance();
private:
    ActionManager(QObject *parent = 0);
    ~ActionManager();

    friend class Core::Internal::CorePlugin; // initialization
    friend class Core::Internal::MainWindow; // saving settings and setting context
};

static ActionManager *m_instance = 0;
ActionManager *ActionManager::instance()
{
    return m_instance;
}
```

所有才有了后面两个友元类的声明了，这样可以直接访问并且初始化对象实例了，核心插件初始化完成后，其它地方可以直接调用单例函数了


接着就是主界面初始化，初始化 `mainWindow`实例
```
    m_mainWindow = new MainWindow;
    if (args.overrideColor.isValid())
        m_mainWindow->setOverrideColor(args.overrideColor);
    qsrand(QDateTime::currentDateTime().toTime_t());
    const bool success = m_mainWindow->init(errorMessage);
```

主界面实例初始化后，接着会调用主界面的初始化函数，主界面真正初始化了多插件界面实现，如果想要学习多插件界面是如何实现的，可以重点关注下这个初始化函数

最后是编辑模式、查找相关功能初始化，这些功能不是本次重点，后面有需要再详细看实现思想

### 主界面初始化

主界面和我们平时创建项目使用的`QMainWindow`没有两样，最大的区别就是`Qt Creator`把界面上所有的操作都进行了封装管理，这样其它插件也可以进行访问，更好的对界面系统进行了扩展

主界面我们重点来学习了菜单栏的使用，看看是如何封装管理的

主要涉及到下面几个类

- ActionContainer
- ActionContainerPrivate
- MenuActionContainer
- MenuBarActionContainer
- ActionManager

这些类的关系如下所示

![](https://gitee.com/devstone/imageBed/raw/master/images/202112192256200.png)

其中 `ActionContainer`对象是基类，向外部所有插件暴露，后面访问某个菜单大部分场景是返回该类指针的

`MenuActionContainer` 是菜单栏当中的菜单对象，可以包含 `n` 个菜单

`MenuBarActionContainer` 是我们的菜单栏，整个 `MainWindows`仅此一份实例

最后就是 `ActionManager`类了，我们所有的操作均是通过该类来进行，很显然它是一个单例类，而且整个系统都是可以访问的

- 创建菜单栏

```C++
ActionContainer *menubar = ActionManager::createMenuBar(Constants::MENU_BAR);

if (!HostOsInfo::isMacHost()) // System menu bar on Mac
    setMenuBar(menubar->menuBar());
```

- 创建文件菜单

```
    ActionContainer *filemenu = ActionManager::createMenu(Constants::M_FILE);
    menubar->addMenu(filemenu, Constants::G_FILE);
    filemenu->menu()->setTitle(tr("&File"));
```

可以看到使用是来是非常方便的，而且这种通过传入字符串创建菜单的方式也简单理解，外部使用的人员完全不用了解函数内部是怎么实现的，只需要根据自己需要传入规定格式的字符串即可

每个菜单都有唯一的字符串 `ID`来进行区分，字符串命名严格按照菜单格式，比如

```
const char M_FILE[]                = "QtCreator.Menu.File";
const char M_FILE_RECENTFILES[]    = "QtCreator.Menu.File.RecentFiles";
const char M_EDIT[]                = "QtCreator.Menu.Edit";
```

这样的格式也很好理解，`Menu`相当于是大菜单，后面一级是每个子菜单，如果该菜单还有子菜单，那么继续扩展下去

其它界面菜单栏菜单创建和上面的过程是类似的，可以照猫画虎写出来

- 创建每个 Action

上面创建了界面的菜单栏，但是每个菜单下面还是空的，需要创建对应的 `Action` 才行，下面来看看是怎么创建的

```C++
    ActionContainer *mfile = ActionManager::actionContainer(Constants::M_FILE);
    
    mfile->addSeparator(Constants::G_FILE_SAVE);

    // Open Action
    QIcon icon = QIcon::fromTheme(QLatin1String("document-open"), Utils::Icons::OPENFILE.icon());
    QAction *m_openAction = new QAction(icon, tr("&Open File or Project..."), this);
    Command *cmd = ActionManager::registerAction(m_openAction, Constants::OPEN);
    cmd->setDefaultKeySequence(QKeySequence::Open);
    mfile->addAction(cmd, Constants::G_FILE_OPEN);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);
```

第一行代码通过菜单管理器返回上面创建的「文件」菜单指针，第二行添加了一个分隔符，后面创建了一个`Command`对象，这个类是对每个`QAction`进行了封装，同时支持设置快捷键等操作，这样我们后续的操作就相当于是一个`command`

这样我们的菜单栏就创建初始化完成了，剩下的就是左侧模式工具条以及中央内容区域的创建了

限于篇幅原因，这些内容我们后面在看

## App 程序初始化

前面花费了大量篇幅来介绍插件的管理以及主界面的实现，下面我们来看看主程序是如何初始化的

主函数 `main.cpp` 里面的内容挺多的，我们看主要加载流程就行了

- 设置系统配置对象指针

```
    QSettings *settings = userSettings();
    QSettings *globalSettings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,
                                              QLatin1String(Core::Constants::IDE_SETTINGSVARIANT_STR),
                                              QLatin1String("QTC"));
    PluginManager pluginManager;
    PluginManager::setPluginIID(QLatin1String("org.qt-project.Qt.QtCreatorPlugin"));
    PluginManager::setGlobalSettings(globalSettings);
    PluginManager::setSettings(settings);
```

主要是系统当中的一些配置，插件管理器需要记录那些插件被禁用了，这样在后面插件初始化时可以跳过了

其中很重要的设置插件`ID`，这个作为插件唯一标识符，用来区分恶意插件，如果别人不知道你的软件插件`IID`，那么他编写的插件放入你程序目录下是可以直接通过这个`IID`过滤掉的


- 设置插件路径

```C++
    const QStringList pluginPaths = getPluginPaths() + customPluginPaths;
    PluginManager::setPluginPaths(pluginPaths);
```

这里的插件路径包含了两部分，一部分是我们程序目录下的插件目录，另一个是公共目录,比如下面这个

```
("D:/CloundCode/QTC/bin/Win32/Debug/QTC/lib/qtc/plugins", "C:/Users/devstone/AppData/Local/QtProject/qtc/plugins/4.4.1")
```

这一步走完后，如果没有错误整个插件都加载完成了

- 异常判断

想法是美好的，但是事实总不如愿，插件在加载过程中可能会遇到一些问题导致加载异常，这样程序就无法正常运行了，需要抛出错误给用户

```C++
    const PluginSpecSet plugins = PluginManager::plugins();
    PluginSpec *coreplugin = 0;
    foreach (PluginSpec *spec, plugins) {
        if (spec->name() == QLatin1String(corePluginNameC)) {
            coreplugin = spec;
            break;
        }
    }
    if (!coreplugin) {
        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1Char(',')));
        const QString reason = QCoreApplication::translate("Application", "Could not find Core plugin in %1").arg(nativePaths);
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (!coreplugin->isEffectivelyEnabled()) {
        const QString reason = QCoreApplication::translate("Application", "Core plugin is disabled.");
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }
```

这段代码对核插件加载状况进行了判断，如果有错误没有加载完成或者被禁用了，那么就直接返回了。理论上来说核心插件是无法被禁用的，但是如果有人恶意修改配置文件禁用了核心插件，那么此时程序会无法正常启动的


- 加载插件

这一步其实是最重要的，上面设置插件路径后仅仅是读取每个插件对象，此时这些对应都是静态的，到了这一步才真正动起来

```C++
PluginManager::loadPlugins();
if (coreplugin->hasError()) {
    displayError(msgCoreLoadFailure(coreplugin->errorString()));
    return 1;
}
```

关于插件加载这个流程最前面插件管理器当中介绍清楚了，这里我们直接略过就行了

好了关于插件加载学习就到这里了


## 总结

插件部分内容还是挺长，初次学习源码的朋友可能会感觉到无从下手、一脸茫然，不用担心，我第一次也是这种感觉，遇到不懂不理解的小标记下，先理解掌握整体设计思想和流程，再慢慢逐个模块攻破

软件开发也是这个道理，一开始你不可能考虑到所有模块细节，把握整体结构没有问题，再挨个实现细节部分

`Qt Creator`非常值得我们多看、多写的，所谓好事多磨么，看的多了也就明白了一些道理

我们日常开发过程中遇到的一些问题，可能`Qt Creator`当中早就实现好了，可以直接拿来使用，比如像奔溃`dump`管理、日志管理、网络通信、特殊控件实现等都可以拿来直接用的

希望本次分享的笔记对你有帮助，如果觉得有用不妨关注下，有任何问题可以互相交流学习



## 相关阅读

- [Qt Creator 源码学习笔记01，初识QTC](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484658&idx=1&sn=55af25cd6e608fa9cb1452610928e71b&chksm=e85c0ac2df2b83d453d80f66fcdefca31e998a8ac0ba0226edbd80b437f99ed5184478805d8e&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记02，认识框架结构结构](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484676&idx=1&sn=c1bd8cbd78d8e098c22353f567cc3620&chksm=e85c0b34df2b822288a7baa016f4a64df47e5e119039836ff686cb2ad3a7e28a5808efc0c13a&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记03，大型项目如何管理工程](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484701&idx=1&sn=c69eb1edfb551c41690938423ca2ef7c&chksm=e85c0b2ddf2b823b1179f216e57ca91b9ce4068a0469e8ba062ab3596e9dc51ac05a1572da85&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记04，多插件实现原理分析](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484767&idx=1&sn=971c14682095a8a84a8161311400100f&chksm=e85c0b6fdf2b82799db871eafb6d0b2465c75d8020b1f87a1f5825f82edcad2c051b7d6e1c2c&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记 05，菜单栏是怎么实现插件化的？](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484822&idx=1&sn=eff40e19952a534fc0d97cab2417ce9e&chksm=e85c0ba6df2b82b017eb568adacd5b407f269cc2c2e7d2c36609fdda76b029bdcd70003fadba&token=193645825&lang=zh_CN#rd)



******

    作者:鹅卵石
    时间: 2021年12月22日23:24:15
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