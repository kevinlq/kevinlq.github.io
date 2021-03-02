---
layout: post
title: 如何使开发的客户端拥有管理员权限
categories: [Qt学习]
description: 记录一下 使用 Qt 开发的客户端添加管理员权限
keywords: Qt, Windows 10, 管理员
---

> 阅读本文大概需要 3 分钟

管理员权限可能大家也比较熟悉，某些软件在运行的时候会弹出安全提示，提示是否以管理员权限运行，点击确定后才能正常运行


随着 Windows 系统的的升级，在 Window 7以上的系统中，我们要读写某些特殊位置可能需要管理员权限，否则会导致读写失败，比如注册表的读写，因此在这种情况下我们需要启动时就要获取管理员权限


给程序设置管理员权限只需要设置应用程序清单配置即可


## 场景描述


上周五使用 Qt 的打包工具打包一个客户端，打包完成后一切都正常，唯独写注册表失败，发现失败的几台电脑都是 Windows7 以上的版本，那么奇怪的问题来了，当时大概猜到了可能是权限的问题


因此，使用鼠标右键单击该安装包，让其以管理员权限运行，结果安装完成后没有一点问题。那么问题就定位到了，Windows 10等系统权限高，需要给我们的安装包赋予管理员权限


## 原理分析
windows系统如何知道某个 exe 运行时以什么权限在运行？其实是通过一个配置文件来确认的，配置文件名字随意起，比如类似这样的 uac.manifest

在我们编译自己的 exe 过程中，将这个配置文件一起编译成最终的 exe 即可


## 准备配置文件


uac.manifest 配置文件的内容大概如下

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1"
          manifestVersion="1.0">
  <!-- Make sure Vista UAC does not believe installerbase is an installer -->
  <trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
    <security>
      <requestedPrivileges>
        <requestedExecutionLevel
          level="requireAdministrator"
          uiAccess="false"/>
      </requestedPrivileges>
    </security>
  </trustInfo>
</assembly>

```
其中最关键的就是这句
```xml
<requestedExecutionLevel level="requireAdministrator" uiAccess="false"/>
```
关于 `level` 类型可以查看微软官方文档


[Microsoft ]("[https://blog.walterlv.com/post/requested-execution-level-of-application-manifest.html#%E5%90%84%E7%A7%8D%E4%B8%8D%E5%90%8C%E7%9A%84-uac-%E6%B8%85%E5%8D%95%E9%80%89%E9%A1%B9](https://blog.walterlv.com/post/requested-execution-level-of-application-manifest.html#%E5%90%84%E7%A7%8D%E4%B8%8D%E5%90%8C%E7%9A%84-uac-%E6%B8%85%E5%8D%95%E9%80%89%E9%A1%B9)", "UAC 类型")


- `asInvoker`
- `requireAdministrator`
- `highestAvailable`
- `requestedExecutionLevel`



上面这四种配置类型可以根据自己需要进行配置


## 工程配置


Qt Pro 配置
```cpp
win32 {
    # Use our own manifest file
    CONFIG -= embed_manifest_exe
    RC_FILE = main.rc
}

```
main.rc 文件内容
```cpp
#define RT_MANIFEST 24
#define CREATEPROCESS_MANIFEST_RESOURCE_ID 1

CREATEPROCESS_MANIFEST_RESOURCE_ID RT_MANIFEST "uac.manifest"

```
然后重新编译你的工程，最后生成的exe启动权限就是管理员权限了，可以查看你的软件 下面是否带一个盾牌图标，如果有那么恭喜你设置成功了，如果没有自己检查下上述几点重要设置


## 总结
这次设置管理员权限是给打包的exe设置了，这样这个exe在把我们的软件打包完成后最终生成新的exe也是拥有同样的权限，由于我们的软件在安装过程中需要写注册表，因此这样刚好解决了我们目前的问题


上述方法是通过修改 `QIFW`源码实现的，如果直接用官方公布的 exe打包是达不到这种效果的






******

    作者:鹅卵石
    时间: 2021年3月2日
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 更多内容敬请关注公众号：devstone

<center>
<img src="/res/img/blog/qrcode_for_devstone.jpg" width="50%" height="50%" />
</center>

---
