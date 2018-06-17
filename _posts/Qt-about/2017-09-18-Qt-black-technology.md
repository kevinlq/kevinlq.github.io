---
layout : post
title: Qt一些黑科技
category : Qt学习
wangyiyun: true
date : 2017-09-18
---

******

    作者:鹅卵石
    时间:2017年9月18日21:27:57
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->


整理整理Qt中的一些**黑科技**

## 1. 编译黑科技

Qt编译给人的感觉就是"慢".其实是我们误解Qt了，想要编译快点，还是有方法的.

### 1.1 使用前置声明

前置声明就是在某个类的头文件中进行声明

B.h
```C++
//进行前置声明类A
class A;

class B{
    public:
        B();
        ~B();

    private:
        A *m_pA;
};
```

b.cpp
```C++
在对应cpp文件中引入头文件
#include "A.h"
...
```

### 1.2 引入编译预处理PCH

在工程.pro文件中添加一行
```C++
PRECOMPILED_HEADER = stable.h
```
需要我们新建一个头文件"stable.h".在该头文件中包含一些其他库的头文件
```C++
//Add C includes file here

#if defined __cplusplus
//Add C++ includes file here

//Qt includes
#include <QtCore>
#include <QtGui>
#include <QtSql>
...

//Custom includes
#include "my_class.h"

...

#endif

```

[Qt官网详细说明-http://doc.qt.io/qt-5/qmake-precompiledheaders.html](http://doc.qt.io/qt-5/qmake-precompiledheaders.html)

### 1.3 多线程编译

使用QtCreatorIDE在windows下用Mingw编译时，可以在make和clean参数中添加-r,这样会直接开始编译，不会等待，需要多线程时可以在添加-j4，编译速度立马上去.

<center>
<img src="/res/img/blog/Qt学习/build-setting.png" width="50%" height="50%" />
</center>

编译时CPU达到100%(我的笔记本是4核的)

<center>
<img src="/res/img/blog/Qt学习/build-j4.png" width="50%" height="50%" />
</center>

## 2. pro相关配置
Qt 中的 `pro` 工程文件可以说是很强大的，好多参数可以直接在这里边进行配置，这里记录最为常用的配置。

区分不同平台不同编译模式生成库以及中间文件路径

**场景**

> 很多时候选择用 Qt 就是看中了他的跨平台能力，所以一个良好的工程配置是少不了的，一个项目要跨平台，就需要针对不同平台进行配置，有时候引入好多第三方库(静态库，动态库)，不同编译模式下的库，添加方式搞错的话会浪费很多时间在排查错误上。

**方案**

新建一个 `xxx.pri` 文件，针对不同平台编写变量，然后再 `xxx.pro` 文件中进行添加即可.

- 区分不同的编译模式，以及生成库的后缀(区分动态库和静态库)

```C++	
CONFIG(debug, debug|release):{
    FILE_POSTFIX = D
    DIR_COMPILEMODE = Debug
}
else:CONFIG(release, debug|release):{
    FILE_POSTFIX =
    DIR_COMPILEMODE = Release
}
```

- 区分平台和编译器

有时候就算同一平台，编译器不同，也要区分

```C++
win32:{
    CONFIG(MinGW, MinGW|MSVC):{
            DIR_COMPILER = MinGW
            FILE_LIB_PREFIX = lib
            FILE_LIB_EXT = .a
    }
    else:CONFIG(MSVC, MinGW|MSVC):{
            DIR_COMPILER = MSVC
            FILE_LIB_PREFIX =
            FILE_LIB_EXT = .lib
    }

    DIR_PLATFORM = Win32
    FILE_DLL_PREFIX =
    FILE_DLL_EXT = .dll
xxx_static:{
    # 始终编译成静态库
    CONFIG += staticlib
    }
}
else:android:{
    CONFIG(ARM_GCC_4.4.3, ARM_GCC_4.4.3|ARM_GCC_4.6|ARM_GCC_4.7|ARM_GCC_4.8):{
        DIR_COMPILER = ARM_GCC_4.4.3
    }
    .....
    .....
    DIR_PLATFORM = Android
    FILE_LIB_PREFIX = lib
    FILE_LIB_EXT = .a
    FILE_DLL_PREFIX = lib
    FILE_DLL_EXT = .so
}
其他平台类似
...

# 定义目标文件生成路径
DESTDIR  = $$PWD/../../Bin/$${DIR_PLATFORM}/$${DIR_COMPILER}/$${DIR_COMPILEMODE}
# 定义目标依赖路径
DIR_DEPEND_DEST = $$PWD/../../Bin/$${DIR_PLATFORM}/$${DIR_COMPILER}/$${DIR_COMPILEMODE}

```

上面定义了好多的变量，有什么用，当然是为了在 `pro` 文件中使用了:

```C++
LIBS += -L$${DIR_DEPEND_DEST} -lQLog$${FILE_POSTFIX}
LIBS += -L$${DIR_DEPEND_DEST} -lQDicom$${FILE_POSTFIX}

PRE_TARGETDEPS += $${DIR_DEPEND_XX_DEST}/$${FILE_LIB_PREFIX}xxx$${FILE_POSTFIX}$${FILE_LIB_EXT}
```

这样配置后，当我们切换到不同平台或者说是不同模式下时就不需要修改库的位置了，会自动加载对应平台、模式的库或者文件了，很是方便.

经过上述配置，会在对应目录下生成比较良好的目录结构:

![](/res/img/blog/Qt学习/Qt-black/Qt-black.png)

这样就不会为了一些配置错误而耽误好多调试时间了.

## 3. 自动生成项目文档
Qt 给我们提供了很大的方便，其中可以帮助我们自动生成文档，可以说是开发维护效率更高吧。

可以想象到，当我们每次修改代码接口后都需要找到对应的文档修改说明，是很麻烦的，恰好可以使用QDoc解决。

我们借助于Qdoc可以生成软件文档，它从我们工程源文件中读取 qdoc 注释，并以 html 或者 xml 形式输出到指定文件夹中.

qdoc只在 .cpp 和 .qdoc 文件中查找注释，不会去 .h 文件中查找.一条 qdoc 注释必须以 ! 符号开始！

### 3.1 使用方式

在需要注释的类或者方法上边进行如下注释:

```C++
/*!
 * \a parent this \a valuse is parent param to
 * the QWidget constructor.
 * \c This c param
 * \class MainWindow
 * \brief MainWindow::MainWindow
 * this blod 加粗文字    \b {blod text}
 * this file
 */
```

### 3.2 比较

**有点**

- 通过qdoc自动提取注释并生成开发文档；
- 可以直接生成接口文档，没有必要单独编写；
- 将代码逻辑写到注释中，便于修改以及同步文档；
- 支持多种格式输出:qch, html;

**缺点**

- 必须按照qdoc指定格式编写注释，并且注释和代码一致；
- qdoc 不能生成私有函数注释输出；

### 3.3 使用

新建文件

xxxdoc.qdocconf

```
project = qtdocs
description = First QDoc Exmaple
outputdir = html
headerdirs = .
sourcedirs = .
outputformats = HTML
sources.fileextensions = "*.cpp *.qdoc *.mm *.qml"
headers.fileextensions = "*.h *.ch *.h++ *.hh *.hpp *.hxx"
outputencoding = UTF-8
sourceencoding = UTF-8
```

在你的工程文件 xx.pro中，添加如下格式文件:

```C++
```

执行命令:

```
qdoc.exe xxx\xx\doc.qdocconf
```

## 4. 编译过程拷贝文件

很多时候项目中用到了其他库文件或者一些资源配置文件，需要编译前拷贝到对应路径(一般是可执行文件路径)，有的需要编译后程序运行时拷贝，手动拷贝太麻烦，加上如果让顾客自己编译，可能会搞不清，突然发现 `QMake` 早就为我们考虑到了，可以通过在 `pro` 文件中进行配置即可.废话不多，show code.

定义原始文件路径(即需要拷贝的文件)、目标路径(一般是可执行程序路径)
```C++
SRC_FILE =$$PWD/../../../RHDRes/LogProperty.conf
DST_FILE =$${DIR_DEPEND_DEST}

```

编译前拷贝
```C++
win32 {

    SRC_FILE~=s,/,\\,g
    DST_FILE~=s,/,\\,g  

    system(copy /y $$SRC_FILE $$DST_FILE)

}

unix {

    system(cp -r -f $$SRC_FILE $$DST_FILE)

}
```


编译后拷贝
```C++
win32:{
    SRC_FILE~=s,/,\\,g
    DST_FILE~=s,/,\\,g
    QMAKE_PRE_LINK +=copy $$SRC_FILE $$DST_FILE
}
unix:{
    QMAKE_PRE_LINK +=cp -r -f $$SRC_FILE $$DST_FILE
}

```

以上命令唯一区别就在拷贝命令`QMAKE_PRE_LINK` 和 `system`.


### 参考

- [配置](https://blog.csdn.net/liulihuo_gyh/article/details/80060838)
- [添加到Qt Assistant中](http://www.cnblogs.com/ll965452300/p/6721493.html)