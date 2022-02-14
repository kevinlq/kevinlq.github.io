---
layout: post
title: Qt Creator 源码学习笔记05，菜单栏是怎么实现插件化的?
categories: [Qt]
description: Qt Creator 源码学习笔记 05
keywords: Qt, 源码,Qt Creator
---

> 阅读本文大概需要 6 分钟

对于一个多插件的 `IDE` 软件来说，支持界面扩展是必不可少的，今天我们来看看在 `Qt Creator` 当中是如何实现界面扩展的

## 概述

界面扩展无非就是在其它插件中访问修改主界面当中的一些菜单、参数，或者添加、删除某些菜单，目前很多大型软件都是支持插件化开发的

前几篇我们一起看了`Qt Creator`的主界面其实很简单，主界面包括一个菜单栏，模式工具栏，内容区域以及状态栏，如下图所示：

![](https://gitee.com/devstone/imageBed/raw/master/images/202112192158402.png)

我们看到的其它丰富功能均是通过插件化实现的，今天我们详细学习下看看 `QTC` 当中菜单栏是怎么实现扩展的


## 实现原理

> 在学习代码之前我们可以想一想，如果让我们自己来实现应该如何实现，比如扩展一个`Menu`菜单？

既然其他插件要扩展，那么肯定需要访问核心插件创建的 `menu` 对象，那么就必须要有访问权限，那么核心插件定义的 `menu` 对象应该有哪些权限呢？

![](https://gitee.com/devstone/imageBed/raw/master/images/202202082214583.png)

仔细回忆下我们刚开始学习 `C/C++` 的时候老师就给我们说过，定义一个变量/对象要注意哪些关键点？

- 变量/对象的名
- 变量/对象的值
- 变量/对象的作用域
- 变量/对象的生命周期

所以我们要实现一个菜单也是需要考虑这几个方面，最关键的是这个对象的生命周期，外部要能访问该对象可以有好几种方式：暴露指针给外使用、提供注册接口、定义单例……，其实把 `menu`定义成一个单例是最便捷最灵活的一种方式了，类似下面这种

```C++
class MenuManager
{
    public:
    static MenuManager * instance();
    
    ......
}
```

PS: 定义接口或者暴露指针也可以，只不过每次访问还要先访问核心插件对象，处理起来比较繁琐罢了

### 源码实现

好了，下面我们看下源码是怎么实现的

菜单管理代码主要在这个位置 : /Src/plugins/.coreplugin/actionmanager

![](https://gitee.com/devstone/imageBed/raw/master/images/202202082217903.png)

文件虽然看着很多，不用担心，我们主要关心的类有这么几个：

- ActionContainer
- ActionContainerPrivate
- MenuActionContainer
- MenuBarActionContainer
- ActionManager

这几个类之间继承关系如下所示：

![](https://gitee.com/devstone/imageBed/raw/master/images/202202082220274.png)

黄色表示的类对内使用，外部看不到具体的实现，每个菜单都可以是一个 `MenuActionContainer` 对象，`MenuBarActionContainer`全局只有一份，相当于是一个容器来容纳所有的菜单

那么我们如何创建一个菜单呢？其中有专门管理创建、注册的类来实现，这是一个单例类

```C++
class CORE_EXPORT ActionManager : public QObject
{
    Q_OBJECT
public:
    static ActionManager *instance();
    
    // 注册菜单
    static ActionContainer *createMenu(Id id);
    
    // 注册菜单栏
    static ActionContainer *createMenuBar(Id id);
    
    // 注册管理某个action
    static Command *registerAction(QAction *action, Id id,
                                   const Context &context = Context(Constants::C_GLOBAL),
                                   bool scriptable = false);
    static void unregisterAction(QAction *action, Id id);
    
    ......
}
```

在这个单例类当中，主要有两个重要的数据结构用来存储创建的菜单对象，详细实现都在它的 `D`指针里面
```C++
class ActionManagerPrivate : public QObject
{
    Q_OBJECT
public:
    typedef QHash<Id, Action *> IdCmdMap;
    typedef QHash<Id, ActionContainerPrivate *> IdContainerMap;
    ......
    
    IdCmdMap m_idCmdMap;
    IdContainerMap m_idContainerMap;
}
```

使用哈希Map 来存储每个对象，当创建的菜单对象比较多时查找效率非常高，同时注意键值`key` 是一个自定义的字符串`ID`，由特殊规则构成的全局唯一的值

```C++
// 创建菜单
ActionContainer *ActionManager::createMenu(Id id)
{
    // 创建前先进行查找，已经存在了直接返回该对象
    const ActionManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it !=  d->m_idContainerMap.constEnd())
        return it.value();

    MenuActionContainer *mc = new MenuActionContainer(id);

    d->m_idContainerMap.insert(id, mc);
    
    // 绑定销毁信号，当菜单对象删除后从当前map中移除
    connect(mc, &QObject::destroyed, d, &ActionManagerPrivate::containerDestroyed);

    return mc;
}

void ActionManagerPrivate::containerDestroyed()
{
    ActionContainerPrivate *container = static_cast<ActionContainerPrivate *>(sender());
    m_idContainerMap.remove(m_idContainerMap.key(container));
}
```

其中有一个比较重要的数据结构 `Context`

```C++
class CORE_EXPORT Context
{
public:
    Context() {}

    explicit Context(Id c1) { add(c1); }
    Context(Id c1, Id c2) { add(c1); add(c2); }
    Context(Id c1, Id c2, Id c3) { add(c1); add(c2); add(c3); }
    ......
    void add(const Context &c) { d += c.d; }
    void add(Id c) { d.append(c); }

private:
    QList<Id> d;
};
```
这个类其实就是一个字符串 `ID` 的数组封装，各个菜单的标识、状态控制都用到了它，这个结构贯穿整个 `Qt Creator`插件系统，使用起来还是非常方便的


有了上面的结构，那么如何创建菜单以及子菜单呢，下面我们详细看下

#### 创建 MenuBar

```C++
    ActionContainer *menubar = ActionManager::createMenuBar(Constants::MENU_BAR);
    // System menu bar on Mac
    if (!HostOsInfo::isMacHost()) 
    {
        setMenuBar(menubar->menuBar());
    }
```

这里没啥好说的，和我们平时在`QMainWindow`当中创建方法一样，只不过这里创建细节统一封装管理起来了

#### 创建菜单

下面我们以「文件」菜单为例看下创建过程

![](https://gitee.com/devstone/imageBed/raw/master/images/202202082238269.png)

```C++
    // File Menu
    ActionContainer *filemenu = ActionManager::createMenu(Constants::M_FILE);
    menubar->addMenu(filemenu, Constants::G_FILE);
    filemenu->menu()->setTitle(tr("&File"));
```

这两行代码就完成了「文件」菜单的创建，代码很简洁也非常容易理解，这里我们需要注意下几个常量定义技巧

```C++
const char M_FILE[]                = "QtCreator.Menu.File";

// Main menu bar groups
const char G_FILE[]                = "QtCreator.Group.File";

```

所有的菜单都是通过字符串常量来区分的，这个常量相当于现实世界中我们每个人的身份证都是唯一的，而且都是有规律的

PS：看到这里再问大家一个问题，定义常量时，宏定义写法和上面的写法哪个好？为什么？欢迎讨论

```C++
#define G_FILE "QtCreator.Group.File"

const char G_FILE[]                = "QtCreator.Group.File";
```

到了这里，仅仅是创建了菜单，点击菜单后内容还是空的，我们接着继续看

```C++

void MainWindow::registerDefaultActions()
{
    // 从单例类中获取上一步创建的菜单容器类 
    ActionContainer *mfile = ActionManager::actionContainer(Constants::M_FILE);
    
    // 添加分隔符
    mfile->addSeparator(Constants::G_FILE_SAVE);
    mfile->addSeparator(Constants::G_FILE_PRINT);
    mfile->addSeparator(Constants::G_FILE_CLOSE);
    mfile->addSeparator(Constants::G_FILE_OTHER);
    
    // 创建每个action
    QIcon icon = QIcon::fromTheme(QLatin1String("document-new"), Utils::Icons::NEWFILE.icon());
    m_newAction = new QAction(icon, tr("&New File or Project..."), this);
    cmd = ActionManager::registerAction(m_newAction, Constants::NEW);
    cmd->setDefaultKeySequence(QKeySequence::New);
    mfile->addAction(cmd, Constants::G_FILE_NEW);
    
    ......
}
```
每个`action`创建后通过 `addAction` 添加到对应的菜单上即可，如果某个 `action` 还有子菜单，那么就需要先创建一个菜单，然后直接添加菜单即可，比如「最近访问的文件」

![](https://gitee.com/devstone/imageBed/raw/master/images/202202082311314.png)

```C++
    ActionContainer *ac = ActionManager::createMenu(Constants::M_FILE_RECENTFILES);
    mfile->addMenu(ac, Constants::G_FILE_OPEN);
    ac->menu()->setTitle(tr("Recent &Files"));
    ac->setOnAllDisabledBehavior(ActionContainer::Show);
```

任意一个`action`可以拥有多个子菜单，只需要在创建的时候根据递归关系选择创建`action`还是`ActionContainer`

## 测试

为了验证上述流程分析是否正确，我们可以编译一个测试插件，然后在该插件里面新创建一个菜单，分为下面几个流程：

- 创建测试插件`PluginDemo`子工程；
- 在插件初始化函数当中创建菜单；
- 编译该插件，然后把该插件(动态库)拷贝到 `QTC` 对应插件目录下
- 运行软件

创建插件编译后生成的目录结构如下所示：
![](https://gitee.com/devstone/imageBed/raw/master/images/202202132133546.png)

可以看到我们测试插件路径和程序 `exe`是独立的

运行软件显示效果如下所示
![](https://gitee.com/devstone/imageBed/raw/master/images/202202102221250.png)


可以看到整个代码不超过 10行就把创建的菜单添加到了主界面当中，使用起来目前看来还是很方便的，而且方便扩展，由于使用插件化和其它模块进行了解耦

相信大家也都看到了，`QTC` 插件系统当中比较重要的`ID`编号问题，这些编号都有固定的格式，而且每个`ID`无论从命名还是具体内容表达的意思都是显而易见的

```C++
const char M_FILE[]                = "QtCreator.Menu.File";
const char M_EDIT[]                = "QtCreator.Menu.Edit";
const char M_EDIT_ADVANCED[]       = "QtCreator.Menu.Edit.Advanced";
const char M_TOOLS[]               = "QtCreator.Menu.Tools";

const char G_FILE_NEW[]            = "QtCreator.Group.File.New";
const char G_FILE_OPEN[]           = "QtCreator.Group.File.Open";
const char G_FILE_PROJECT[]        = "QtCreator.Group.File.Project";
const char G_FILE_SAVE[]           = "QtCreator.Group.File.Save";
```

- `M`开头表示菜单名字，比如文件、编辑、视图、构建……
- `G`开头表示分组信息，比如文件菜单当中包含了：新建文件、打开文件、打开工程、保存文件……


## 总结

`Qt Creator`界面插件化内容还很多，本次只是简简单单地学习了菜单管理逻辑以及如何使用，如果想了解更多细节阅读对应源码即可

一款优秀的开源软件有很多内容值得我们反复去学习、理解、使用的，未来很长，我们继续……


---


PS：文中涉及到相关流程图以及对应源码，如果感兴趣可以后台私信发给你

如果觉得对你有帮助，欢迎留言互相交流学习


## 相关阅读

- Qt Creator 学习笔记01，初识 QTC[^1]
- Qt Creator 源码学习笔记02，认识框架结构[^2]
- Qt Creator 源码学习笔记03，大型项目如何管理工程[^3]
- Qt Creator 源码学习笔记04，多插件实现原理分析[^4]
- Qt Creator 源码学习笔记 05，菜单栏是怎么实现插件化的[^5]


[^1]: https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484658&idx=1&sn=55af25cd6e608fa9cb1452610928e71b&chksm=e85c0ac2df2b83d453d80f66fcdefca31e998a8ac0ba0226edbd80b437f99ed5184478805d8e&token=2125642557&lang=zh_CN#rd
[^2]: https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484676&idx=1&sn=c1bd8cbd78d8e098c22353f567cc3620&chksm=e85c0b34df2b822288a7baa016f4a64df47e5e119039836ff686cb2ad3a7e28a5808efc0c13a&token=2125642557&lang=zh_CN#rd
[^3]:https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484701&idx=1&sn=c69eb1edfb551c41690938423ca2ef7c&chksm=e85c0b2ddf2b823b1179f216e57ca91b9ce4068a0469e8ba062ab3596e9dc51ac05a1572da85&token=2125642557&lang=zh_CN#rd
[^4]:https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484767&idx=1&sn=971c14682095a8a84a8161311400100f&chksm=e85c0b6fdf2b82799db871eafb6d0b2465c75d8020b1f87a1f5825f82edcad2c051b7d6e1c2c&token=2125642557&lang=zh_CN#rd
[^5]https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484822&idx=1&sn=eff40e19952a534fc0d97cab2417ce9e&chksm=e85c0ba6df2b82b017eb568adacd5b407f269cc2c2e7d2c36609fdda76b029bdcd70003fadba&token=193645825&lang=zh_CN#rd

******

    作者:鹅卵石
    时间: 2022年2月14日21:43:20
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