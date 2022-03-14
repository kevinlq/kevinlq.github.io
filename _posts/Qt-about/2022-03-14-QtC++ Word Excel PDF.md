---
layout: post
title: C++(Qt) 和 Word、Excel、PDF 交互总结
categories: [Qt]
description: C++(Qt) 和 Word、Excel、PDF 交互总结
keywords: Qt, C++,Word, Excel, PDF
---


> 阅读本文大概需要 6 分钟

日常开发软件可能会遇到这类小众需求，导出数据到 `Word`、`Excel` 以及 `PDF`文件，如果你使用 `C++` 编程语言，那么可以选择的方案不是很多，恰好最近刚好有这部分需求，整理下这段时间踩过的坑，方便后人

## 读写 Word

日常开发的软件使用最多的应该是导出数据到 `Word` 文档中，目前可以用的方案有这几种

![](https://gitee.com/devstone/imageBed/raw/master/images/202203131456920.png)

没有十全十美的方案，任何方案都存在优点和缺点，下面来详细看下这几种方案的优缺点以及适用场景

### XML 模板替换

> 原理：事先编辑好一份 `Word` 模板，需要替换内容的
地方预留好位置，然后使用特殊字段进行标记，后面使用代码进行全量替换即可完成

#### 优点

- 代码量相对较少、导出速度快
- 跨平台，支持多个系统，系统不安装 office 也能导出；
- 支持图片以及固定格式导出；

#### 缺点

- 导出格式固定，可扩展性不强，如果需求变化导出格式变了，那么模板也要跟着改变；
- 一种格式对应一份模板，如果导出格式较多，需要准备的模板文件较多，这样比较繁琐；
- 需要 `Word` 2003 以上版本；

#### 举个栗子

我们先编辑一份 `Word` 模板文档，内容大概如下所示：

![](https://gitee.com/devstone/imageBed/raw/master/images/202203131659396.png)

- 将该文档另存为 `Word XML` 文档 `XML-Template.xml`
- 读取文档内容进行变量替换

```C++
    QFile file("XML-Template.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open xxml file fail. " << file.errorString();
        return 0;
    }
    QByteArray baContent = file.readAll();
    file.close();
    QString strAllContent = QString::fromLocal8Bit(baContent);

    strAllContent.replace("$VALUE0", "1");
    strAllContent.replace("$VALUE1", QString::fromLocal8Bit("法外狂徒张三"));
    strAllContent.replace("$VALUE2", QString::fromLocal8Bit("考试不合格"));
    strAllContent.replace("$VALUE3", "2");
    strAllContent.replace("$VALUE4", QString::fromLocal8Bit("李四"));
    strAllContent.replace("$VALUE5", QString::fromLocal8Bit("合格"));

    QFile newFile("export.doc");
    if (!newFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "file open fail." << newFile.errorString();;
        return 0;
    }

    newFile.write(strAllContent.toLocal8Bit());
    newFile.close();
```

- 保存替换后的内容,写入文件

![](https://gitee.com/devstone/imageBed/raw/master/images/202203131827960.png)

可以看出来这种方式比较繁琐，重点是编辑固定的模板格式，而且编辑好后保存成`XML`格式后还需要继续调整，这种 `XML` 格式标签很多，不小心就修改错了，导致导出的文档打不开

这种方式适合模板内容不太复杂，内容较少的情况下使用

### COM 组件方式

> 原理：采用 `Micro Soft`公开的接口进行通讯，进行读写时会打开一个 `Word进程来交互

[COM 技术概述](https://docs.microsoft.com/zh-cn/windows/win32/com/com-technical-overview)

`Qt` 为我们提供了专门进行交互的类和接口，使用 `Qt ActiveX`框架就可以很好的完成交互工作

#### 优点

- 实现简单，快速上手；


#### 缺点

- 导出写入速度慢，因为相当于打开 word 文档操作；
- 仅 `Windows`平台可用，其它平台失效；
- 需要程序运行的电脑安装 office word，否则调用失败

#### 举个栗子

使用时需要引入对应的模块，在 `pro` 文件引入模块

```
QT  *= axcontainer
```

打开文档写入内容

```C++
QAxObject *pWordWidget = new(std::nothrow) QAxObject;

bool bResult = pWordWidget->setControl("word.Application");

if (!bResult)
{
    return false;
}

// 设置是否显示
pWordWidget->setProperty("Visible", false);

QAxObject *pAllDocuments = pWordWidget->querySubObject("Documents");

if(nullptr == pAllDocuments)
{
    return false;
}

// 新建一个空白文档
pAllDocuments->dynamicCall("Add (void)");

// 获取激活的文档并使用
QAxObject *pActiveDocument = pAllDocuments->querySubObject("ActiveDocument");
if(nullptr == pActiveDocument)
{
    return false;
}

// 插入字符串
QAxObject *pSelectObj = pWordWidget->querySubObject("Selection");
if (nullptr != pSelectObj)
{
    pSelectObj->dynamicCall("TypeText(const QString&)", "公众号：devstone");
}

……
```

可以看出来使用起来不难，对于新手友好一点，很多写入操作方法比较繁琐，需要自己重新封装一套接口

- 这种方案比较适合那些排版比较复杂，图片、文字、表格混排的场景下，而且内容都是动态变化的，可以很好的实现定制化

- 当然了它的缺点也不少，也有一些坑，有时候莫名其妙会失败，还有就是比如你电脑安装的 `Word` 没有激活，那么每次启动会弹激活窗口
- 还有就是这种方式要求所有的路径必须是本地化的，比如 `D:\\Soft\test.png`
- 使用前最好读取注册表判断当前电脑是否安装了 `Office Word`，如果没有安装，直接读取操作肯定会崩溃

这种方式同样适用于写入 `Excel` 文件，后面再说

### HTML 方式

> 原理：这种方式得益于 `Word`支持 HTML格式导出渲染显示，那么反向也可以支持，需要我们拼接 `HTML`格式内容，然后写入文件保存成 `.doc`格式

#### 优点

- 跨平台，不仅限于 `Windows`平台，代码可扩展性比较好
- 导出速度快、代码可扩展；

#### 缺点
- 字符串拼接 `HTML` 容易出错，缺失标签导出后无法显示；
- 插入的图片是本地图片文件的链接，导出的 word文档拷贝到其它电脑图片无法显示

#### 举个栗子

```C++
QString HTML2Word::getHtmlContent()
{
    QString strHtml = "";
    strHtml += "<html>";
    strHtml += "<head>";
    strHtml += "<title>测试生成word文档</title>";
    strHtml += "<head>";
    strHtml += "<body style=\"bgcolor:yellow\">";
    strHtml += "<h1 style=\"background-color:red\">测试qt实现生成word文档</h1>";
    strHtml += "<hr>";
    strHtml += "<p>这里是插入图片<img src=\"D:\\title.jpg" alt=\"picture\" width=\"100\" height=\"100\"></p>";
    strHtml += "</hr>";
    strHtml += "</body>";
    strHtml += "</html>";

    return strHtml;
}

// 保存写入文件
QFile file("D:/htmp2Word.doc");
if (!file.open(QIODevice::WriteOnly))
{
    return false;
}

QTextStream out(&file);
out << getHtmlContent();
file.close();

```

![](https://gitee.com/devstone/imageBed/raw/master/images/202203131924019.png)

这种方式难点在于 `HTML`格式拼接，任何缺失字段都会导致导出失败，适合小众需求下导出

图片问题其实可以手动进行转化，文档导出成功后手动拷贝内容到新的文档，这样图片就真正插入到文档中，文档发送给别人也不会丢失图片了

还有一个坑就是：如果你使用 `WPS` 打开导出的文档，默认显示的是 `web`视图，需要手动进行调整

某些电脑分辨率变化也会导致生成的文档中字体等产生变化


### 第三方开源库

可以使用的第三方库几乎没有，网络上找到的有这么几个

- OpenOffice: 兼容性差，集成调用难度大
- LibOffice: 太庞大，不容易集成
- DuckX： 太小众，只能简单的使用
- docx：小众库

[DuckX库](https://github.com/amiremohamadi/DuckX)
[docx库](https://github.com/lpxxn/docx)

在读写 `Word`这部分，`C++` 基本没有可以使用的第三方库，不像其他语言`Java`、`C#`、`Python`有很多可以选择，这个痛苦也只有 `C++` 程序员能够理解了吧

所以怎么选择还是看自己项目需求吧，没有十全十美的方案


---

上面说了这么多，都是导出生成 `Wrod`，那么下面来看看有那些方式可以读取显示 `Word`内容

这种需求应该不会很多，而且显示难度更大一些

使用 `COM`组件方式，即采用 `QAxWidget`框架显示 `
office` 文档内容，本质上就是在我们编写的 `Qt` 界面上嵌入 `office` 的软件，这种方式其实和直接打开 `Word`查看没有啥区别，效果、性能上不如直接打开更好一些

目前一般都会采用折中方案，把 `Word` 转为 `PDF` 进行预览加载显示，我们知道 `PDF` 渲染库比较多，生态相对来说要好一些，在选择上就更广泛些，如何使用后面部分有专门介绍 `PDF`章节

## 读写 Excel

目前有一个支持比较好的第三方库可以使用，整体使用基本可以满足日常使用

[QXlsx](https://github.com/QtExcel/QXlsx)

![](https://gitee.com/devstone/imageBed/raw/master/images/202203132115621.png)

这款开源库支持跨平台，Linux、Windows、Mac、IOS、Android，使用方式支持动态库调用和源码直接集成，非常方便

编译支持 `qmake`和`cmake`，可以根据你自己的项目直接集成编译，读写速度非常快

```C++
QXlsx::Document xlsx;

// 设置一些样式
QXlsx::Format titleFormat;
titleFormat.setBorderStyle(QXlsx::Format::BorderThin);  // 边框样式
titleFormat.setRowHeight(1,1,30);   // 设置行高
titleFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);   // 设置对齐方式

// 插入文本
xlsx.write(1,1, "微信公众号：devstone", titleFormat);

// 合并单元格
xlsx.mergeCells(QXlsx::CellRange(2,1,4,4), titleFormat);

// 导出保存
xlsx.saveAs("D:/xlsx_export.xlsx");

// 添加工作表
xlsx.addSheet("devstone");
```

可以看到上手非常容易、各个函数命名也贴近 `Qt Api`，是一款非常良心的开源软件

> PS:注意该软件使用 `MIT` 许可协议，这样对于很多个人或者公司来说非常良心，意味着你可以无偿使用、修改该项目，但是必须在你项目中也添加同样的 `MIP`许可

上面也提到了，还可以使用 `COM` 组件的方式读写 `Excel`，不过有了这款开源库基本就可以告别 `COM`组件方式了

## 读写 PDF

`PDF`相关开源库挺多的，给了 `C++` 程序员莫大的帮助，目前可用的主要有这些

![](https://gitee.com/devstone/imageBed/raw/master/images/202203132237709.png)

其中 `mupdf`和 `poppler` 属于功能强大但是很难编译的那种，需要有扎实的三方库编译能力，否则面对 `n` 个依赖库会无从下手

不过可喜的是 `Github` 上有两个开源库可以供选择

### qpdf 库

这个库其实封装了 `pdf.js`库，使用 `WebEngine`来执行 `JavaScript`进而加载文件

![](https://gitee.com/devstone/imageBed/raw/master/images/202203132335058.png)

[项目地址](https://github.com/Archie3d/qpdf)

- 直接从本地文件加载；
- 支持从内存数据直接加载渲染 PDF 内容；

这种方式对环境有特殊要求了，如果你的项目使用的 `Qt` 版本不支持 `WebEngine`，那么就无法使用

### qtpdf 库

这个库是 `Qt` 官方亲自操刀对第三方库进行了封装，暴露的 `API` 和 `Qt` 类似，使用起来非常舒服

[Qt 官方](https://www.qt.io/blog/2017/01/30/new-qtpdf-qtlabs-module)

![](https://gitee.com/devstone/imageBed/raw/master/images/202203132252485.png)

代码结构以及使用 Demo
![](https://gitee.com/devstone/imageBed/raw/master/images/202203132258364.png)

#### 小试牛刀

关于如何使用，官方已经给了我们非常详细的步骤了，直接跟着下面几步就 OK 了

[官方教程](https://wiki.qt.io/Handling_PDF)

```C++
git clone git://code.qt.io/qt-labs/qtpdf
cd qtpdf
git submodule update --init --recursive
qmake
make
cd examples/pdf/pdfviewer
qmake
make

./pdfviewer /path/to/my/file.pdf
```
可以看到使用了谷歌开源的 `pdfium` 三方库，编译时需要单独更新下载这个库，因为某些原因可能你无法下载，不过好在有人在 `GitHub`上同步了这个仓库的镜像，有条件还是建议直接下载最新稳定版的

![](https://gitee.com/devstone/imageBed/raw/master/images/202203132302785.png)

可正常访问的仓库地址：https://github.com/PDFium/PDFium

相关类可以看这个文档：https://developers.foxit.com/resources/pdf-sdk/c_api_reference_pdfium/modules.html


> 最后还要注意项目开源协议：pdfium引擎开始来自于福昕，一个中国本土的软件公司，Google与其合作最终进行了开源，目前采用的是 `BSD 3-Clause` 协议,这种协议允许开发者自由使用、修改源代码，也可以修改后重新发布，允许闭源进行商业行为，不过需要你在发布的产品中包含原作者代码中的 `BSD` 协议



## 总结

以上就是项目中常用的文档处理方法总结，当然了肯定也还有其它方案可以实现，毕竟条条大路通罗马，如果你还要不错的方案和建议欢迎留言

PS: 以上方案和对应的源码编译、使用例子会统一上传到 `GitHub`对应的仓库，方便后人使用


取之互联网、回报互联网


原创不易，如果觉得对你有帮助，欢迎点赞、在看、转发

**推荐阅读**

- [Qt Creator 源码学习笔记01，初识QTC](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484658&idx=1&sn=55af25cd6e608fa9cb1452610928e71b&chksm=e85c0ac2df2b83d453d80f66fcdefca31e998a8ac0ba0226edbd80b437f99ed5184478805d8e&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记02，认识框架结构结构](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484676&idx=1&sn=c1bd8cbd78d8e098c22353f567cc3620&chksm=e85c0b34df2b822288a7baa016f4a64df47e5e119039836ff686cb2ad3a7e28a5808efc0c13a&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记03，大型项目如何管理工程](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484701&idx=1&sn=c69eb1edfb551c41690938423ca2ef7c&chksm=e85c0b2ddf2b823b1179f216e57ca91b9ce4068a0469e8ba062ab3596e9dc51ac05a1572da85&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记04，多插件实现原理分析](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484767&idx=1&sn=971c14682095a8a84a8161311400100f&chksm=e85c0b6fdf2b82799db871eafb6d0b2465c75d8020b1f87a1f5825f82edcad2c051b7d6e1c2c&scene=21#wechat_redirect)
- [Qt Creator 源码学习笔记 05，菜单栏是怎么实现插件化的？](https://mp.weixin.qq.com/s?__biz=MzIyNzY5NTQ1NQ==&mid=2247484822&idx=1&sn=eff40e19952a534fc0d97cab2417ce9e&chksm=e85c0ba6df2b82b017eb568adacd5b407f269cc2c2e7d2c36609fdda76b029bdcd70003fadba&token=193645825&lang=zh_CN#rd)


******

    作者:鹅卵石
    时间: 2022年3月14日22:24:15
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