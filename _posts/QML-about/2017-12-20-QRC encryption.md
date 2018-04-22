---
layout: post
title: Qt资源文件加密
categories: [QML学习]
description: 将QML放入资源文件中，接着对资源文件进行加密
keywords: QML,QRC，Qt
---

Qt 中使用资源文件 `qrc`绝对是一大亮点，为我们解决了很多棘手的事情.

qrc中包含了很多的资源文件，比如:图片、QML文件、JS文件……

## 由来

很多时候，当程序中使用的图片太多，慢慢可能会遇到一些错误，比如:
```
fatal error c1076:compiler limit ....
```

虽然可以修改限制大小，但是随着项目越来越大，显然这种办法不可行.

有时候我们写的`QML`代码不想让别人看到，该如何，QML代码又不能编译进exe中(除非进行静态编译)

## 编译qrc

Qt为我们提供了一个工具`rcc`，可以使用该工具对我们的资源文件进行二进制编译，也就是加密，编译后会生成新的文件，然后我们在程序中直接对新的文件进行注册，就可以正常使用了.

### qrc文件
```
<RCC> 
<qresource prefix="/"> 
<file>QmlRes/main.qml</file> 
<file>QmlRes/Common/Button.qml</file> 
<file>QmlRes/Common/LineEdit.qml</file> 
<file>QmlRes/image/home_back.png</file> 
.......
</qresource> 
</RCC> 
```
如上所示就是一个简单的qrc文件内容

### 编译

`rcc` 工具可以从自己Qt安装路径查找.

![rcc工具](/res/img/blog/QML-learn/rcc.png)

### 编译命令

核心命令只有一个:

```
%~dp0\Tool\MinGW\rcc.exe -binary %~dp0\qml.qrc -o %~dp0\Bin\QmlResSkin.rcc
```

可以写一个`bat`处理，加载的时候直接双击即可.

![rcc工具](/res/img/blog/QML-learn/rcc_build.png)

## 使用

使用时，自己在程序启动前进行注册加密后的资源文件.

![rcc工具](/res/img/blog/QML-learn/rcc_use.png)


关键代码

```C++
QResource::registerResource ("QmlResSkin.rcc");
```

### 效果

![rcc工具](/res/img/blog/QML-learn/rcc_register.png)

## 参考脚本

```C
cd QmlRes

echo 当前路径: %~df0

::设置编译后的文件路径
set RCC_Foler=Bin
set RCC_FullFoler=%~dp0\%RCC_Foler%
echo 设置当前编译后的路径为:%RCC_Foler%

::设置编译后的资源文件名
set skinName=QmlResSkin.rcc
echo 设置编译后的资源文件名称为:%skinName%

::检测当前编译后资源路径是否存在
if exist %RCC_FullFoler% (
	echo 文件目录 %RCC_FullFoler%已经存在
)else (
	echo 文件目录 %RCC_FullFoler%不存在,正在创建文件夹
	md %RCC_FullFoler%
	if exist %RCC_FullFoler% (
		echo 已经创建 %RCC_FullFoler% 路径
	)else (
		echo 创建文件路径失败,将退出
		goto stop
	)
)
.....

cd ..

echo 正在编译.....
%~dp0\Tool\MinGW\rcc.exe -binary %~dp0\qml.qrc -o %~dp0\Bin\%skinName%
echo.
echo 编译结束......
```

---
******

    作者:鹅卵石
    时间:2017年12月20日21:36:47
    版本:V 0.0.2
    邮箱:kevinlq@yeah.net

<!-- more -->
>本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。
但是，希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。

**如果您对本文有任何问题，可以在下方留言，或者Email我.**
