---
layout: post
title: Qt在IOS平台编译配置以及打包发布
categories: [工具使用]
description: Qt在IOS平台编译配置以及打包发布
keywords: Qt,tools，IOS
---

在此期间，有幸接触到IOS，所以对Qt 在 IOS上的开发部署有了一些了解，今天拿出笔记记录下简单的过程。

## Xcode安装

MAC上面安装软件很方便也很烦，需要打开App Store进行搜索安装.

![xcode](/res/img/blog/tools/mac_xcode_install.png)


安装好之后会看到如下欢迎界面:

![xcode home](/res/img/blog/tools/xcode_home.png)

表示安装成功，接下来可以安装Qt了(**这里貌似要先安装xcode在安装Qt！**)

## Qt的安装

比如下载Qt5.7版本，地址[http://download.qt.io/archive/qt/5.7/5.7.0/](http://download.qt.io/archive/qt/5.7/5.7.0/)，为了以后方便尽量下载比较全的安装包:**qt-opensource-mac-x64-android-ios-5.7.0.dmg**，直接包含了三个构建套件(MAC,IOS,Android)

![download](/res/img/blog/tools/qt_ios_download.png)

下载完成直接按照默认安装提示安装(傻瓜式安装).

安装好后就可以看到比较熟悉的界面了，主要有看三个套件:

- Desktop Qt 5.7.1 clang 64bit
- iphoneos-clang Qt 5.7.1 for iOS
- iphonesimulartor-clang Qt 5.7.1 for iOS


## 测试

创建一个helloworld工程,这里套件要选择 iphoneos-clang Qt 5.7.1 for iOS,因为是在真机跑，如果选择模拟器，那么要选择iphonesimulartor套件.

编译后会生成xcode工程文件，若要在真机运行，需要使用xcode进行编译打包调试.

工程文件: xxx.xcodeproj


## iOS证书和签名

### 证书
![singal](/res/img/blog/tools/ios_signal.png)

证书分为：开发证书和发布证书.

- 开发证书
安装在电脑上提供权限，开发人员通过设备进行真机测试。
- 发布证书
开发人员可以制作测试版和发布版的程序，不可生成副本，仅有配置该证书的电脑才能使用

>证书生成

开发者在申请iOS开发证书时，需要通过keychain生成一个CSR文件（Certificate Signing Request），提交给苹果的 Apple Worldwide Developer Relations Certification Authority(WWDR)证书认证中心进行签名，最后从苹果官网下载并安装使用。这个过程中还会产生一个私钥。

- 证书本身
包含用户的公钥、用户个人信息、证书颁发机构信息、证书有效期等信息

- 证书签名
WWRD将上述证书本身内容使用哈希算法得到一个固定长度的信息摘要，然后使用自己的公钥对该信息摘要进行加密生成数字签名。


### 签名

Xcode在打包生成ipa文件的过程中，利用当前证书的私钥进行代码、资源文件的数字签名，并且将其放在ipa文件夹的_CodeSignature文件夹下。
当APP安装到iOS系统上时，系统首先通过描述文件找到数字证书，通过证书里面的苹果数字签名，验证证书的有效性，如果证书有效，则取出证书中的开发者公钥，解密APP的数字签名，如果发现摘要一致。则验证通过，APP成功安装到手机上。如果其中某一个验证环节出现问题，验证工作就会失败。

### 证书有效期
iOS打包时需要一个证书和该证书对应的描述文件。描述文件过期后，续费后需要重新生成描述文件，然后重新对ipa进行签名打包才可以，过期后的app是无法使用的，会提示无法信任的企业级应用.


### 导出P12证书

导出P12证书是为了给其他MAC电脑开发用的，因为一个证书只能最多只能在2台电脑上开发。通过导出.p12证书文件可以供给所以的mac设备使用，使设备不需要在苹果开发者网站重新申请和发布证书就能使用。
导出的.p12证书必须和对应的描述文件一起使用才可以。
将导出的.p12文件拷贝到其他mac设备上，直接双击进行安装即可，这样直接就可以安装到钥匙串中了。以后在xcode调试或者重新签名打包时选择刚才安装的证书和对应的描述文件即可。
描述文件是为了让xcode知道我们开发的合法性。

## APP多语言名称设置

参考文章地址[http://www.jianshu.com/p/88c1b65e3ddb](http://www.jianshu.com/p/88c1b65e3ddb).

使用xcode打开项目工程，在项目名称上面单击鼠标右键，在弹出的对话框中选择"New File..

![app language](/res/img/blog/tools/ios_app_language.png)

在弹出的对话框中选择Strings File，如下图所示：

![app language](/res/img/blog/tools/ios_app_language2.png)

接着选择路径以及文件名称，文件名称这里为”InfoPlist.strings”,路径选择默认即可.

创建完成后鼠标点击该文件，添加如下内容：
CFBundleDisplayName =”应用程序”;
在xcode右边属性栏中选择Locakuze…

![app language](/res/img/blog/tools/ios_app_language3.png)

完成后在工程的信息属性栏中，选择Localizations，点击下面的+号，添加中文简体名称，如下图所示：

![app language](/res/img/blog/tools/ios_app_language4.png)

在弹出的对话框中选择上面新建的文件“InfoPlist.strings”。添加完毕后可以看看到”InfoPlist.strings”文件旁边多了一个小三角，点击后可以看到展成2部分，就是刚才添加的中文还有一个默认的英文，点击对应文件进行修改对应名称。

![app language](/res/img/blog/tools/ios_app_language5.png)


## APP名称修改

![app name](/res/img/blog/tools/ios_app_name.png)


## APP图标设置

通过项目images.xcassets文件实现

在工程属性“General”选项中找到“App Icons and Launch Images”项

![app icon](/res/img/blog/tools/ios_app_icon.png)

![app icon](/res/img/blog/tools/ios_app_icon2.png)

击该文件，可以看到有2项，第一个是App图标设置，第二个是启动图标

点击后会出现好几种尺寸的icon选择，这里需要提前准备好对应尺寸的icon图标，然后按照尺寸拖到对应位置上即可。
图标的命名：
注意区分倍图@2x 和@3x 的后缀，如AppIcon57x57.png 和AppIcon57x57@2x.png
AppIcon57x57.png代表图标尺寸为：57*57
AppIcon57x57@2x.png代表图标尺寸为：114*114

**上述图标大小需要提前准备好，然后手动拖到匹配位置即可.**


## 启动图标的设置

启动图标和APP图标类似,分为横屏和竖屏.

![app launch](/res/img/blog/tools/ios_app_launch.png)

- Portrait 代表竖屏
- Landscape代表竖屏


## 关于iphone上下边黑屏问题

参考文章[https://www.jianshu.com/p/5090fc45c271](https://www.jianshu.com/p/5090fc45c271)


## 添加http访问权限

在iOS 9之后，苹果默认要求App访问的url必须为https的安全链接，http链接确实是不安全的，如果在开发过程中请求失败，控制台显示http不安全要用https之类的信息的话，那就是由于这个原因了。但是由于并非所有开发者都会去申请HTTPS证书来支持HTTPS访问，所以还是可以进行设置来正常访问HTTP的，方法如下：

- 在Xcode工程中找到Info.plist文件，做iOS开发的应该都了解这是一个做一些应用配置的文件；
- 在Info.plist文件中添加Dictionary类型的NSAppTransportSecurity字段；
- 在NSAppTransportSecurity字段下添加Boolean类型的NSAllowsArbitraryLoads字段，并将其值设为YES

![app language](/res/img/blog/tools/ios_app_http.png)


## Archive打包

![app language](/res/img/blog/tools/ios_app_package.png)

接着会进行编译，如果没有错误，则会弹出一个对话框—Archive Information:

![app language](/res/img/blog/tools/ios_app_package2.png)

选择Export进行导出，这个时候会弹出对话框，让你选择方式：

![app language](/res/img/blog/tools/ios_app_package3.png)

- 保存到本地 准备上传App Store 或者在越狱的iOS设备上使用，利用的是Distribution描述文件，关联production证书
- 保存到本地 准备在账号添加的可使用设备上使用（具体为在开发者账户下添加可用设备的UDID）,利用的是Distribution描述文件，关联production证书；
- (企业级APP打包ipa)这种主要针对企业级账户下准备本地服务器分发的app。利用的是Distribution描述文件，关联production证书；开发者测试包;
- 发者模式打包ipa,通过development描述文件关联development证书，打包ipa，给注册的UDID机子安装；

>注明：真机连接Xcode进行调试，需要使用development描述文件及code sign用Developer证书，如果使用production描述文件及code sign用Distribution证书，不能改运行调试，报：process launch failed: failed to get the task for process 10487(数字)错误提示！

企业发布的话一般选择第三个:**Save for Enterprise Deployment**.

接着会让你选择自己的Team:

![app language](/res/img/blog/tools/ios_app_package4.png)

接着一路next

![app language](/res/img/blog/tools/ios_app_package5.png)

最后会让你选择一个存放路径，最后会生成一个对应的**xxx.ipa**文件了.

![app language](/res/img/blog/tools/ios_app_package6.png)

**xxx.ipa**文件其实就是一个压缩文件，我们可以将其重新命名为**xxx.zip**，解压后可以看到里面的一些基本内容.


## IPA文件组成

iOS程序最终都会以.ipa文件导出，先来了解一下ipa文件的结构:

![app language](/res/img/blog/tools/ios_app_ipa.png)

- 资源文件，例如图片、html、等等。
- _CodeSignature/CodeResources。这是一个plist文件，可用文本查看，其中的内容就是是程序包中（不包括Frameworks）所有文件的签名。注意这里是所有文件。意味着你的程序一旦签名，就不能更改其中任何的东西，包括资源文件和可执行文件本身。iOS系统会检查这些签名。
- 可执行文件。此文件跟资源文件一样需要签名。
- 一个mobileprovision文件.打包的时候使用的，从MC上生成的。
- Frameworks。程序引用的非系统自带的Frameworks，每个Frameworks其实就是一个app，其中的结构应该和app差不多，也包含签名信息CodeResources文件


## 支持文件共享(读取app的log日志)

info.plist里设置UIFileSharingEnabled = YES;

然后打开iTunes ，找到app,即可看到日志文件


## IOS App在后台运行不退出


因为IOS系统机制吧，锁屏后app的网络会断开连接，所以如果你的app需要一直保持数据通信，则很麻烦，不过短暂的方法可以这样设置:

程序打包时在Info.plist添加如下两个字段即可：

![app language](/res/img/blog/tools/ios_app_runBackground.png)

参考文章地址[https://www.jianshu.com/p/174fd2673897](https://www.jianshu.com/p/174fd2673897)





******

    作者:鹅卵石
    时间:2018年3月11日11:04:06
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**
