---
layout: post
title: IOS IPA 上架过程记录
categories: [工具使用]
description: IOS IPA 上架过程记录
keywords: IPA,appStore
---

了解了下一款IPA如何上架IOS appStore的过程，记录下基本的流程。

>部分内容来自于网络，如有不妥，请联系我删除.谢谢!

## 上架步骤

- 开发者账号(注意区分几种账号的区别);
- 创建APP ID(需要登录到苹果开发者网站去创建);
- 创建发布证书(Certificates);
- 创建发布描述文件(Profiles);
- 安装证书以及导入描述文件(MAC中钥匙串查看);
- XCode工程配置发布证书,准备打包;
- 打包上传app Store;

![账号和证书分类--来自于网络](/res/img/blog/tools/IOS/info.png)

>上述图片来自于网络，感谢.

## 1. 登录开发者账号

[开发者登陆](https://idmsa.apple.com/IDMSWebAuth/login?appIdKey=891bd3417a7776362562d2197f89480a8547b108fd934911bcbea0110d07f757&path=%2Faccount%2F&rv=1)


![](/res/img/blog/tools/IOS/login.png)

如上图所示，填写你的账号信息进行登录，如果没有那么首先进行创建.


## 2. 创建 APP ID

登录成功后，选择 `Certificates,Identifiers &Profiles`选项

![](/res/img/blog/tools/IOS/home.png)

### 2.1 选择 APP IDs

选择右上角 + 新建一个 APP ID;

![](/res/img/blog/tools/IOS/appID.png)

- Name 需要自己填写，用来描述我们的APP ID，最好和自己的工程名称对应起来(不能填写中文)

- Bundle ID,这个是APP ID 的后缀，用来标识app,最好填写工程名对应的 Bundle ID,比如：com.baidu.AppName

- App Services.默认选择2项，不能修改，其他服务根据需要勾选(推送服务、VPN....);

填写完毕后点击 `Continue` 确认，进行下一步

![](/res/img/blog/tools/IOS/add_appID.png)

![](/res/img/blog/tools/IOS/add_appID2.png)

检查下没有错的话直接点击Register后点击Done完成App ID的创建

![](/res/img/blog/tools/IOS/add_appID3.png)


## 3. 创建发布证书

点击All ,再点击右上角 `+` 号，添加一个新的发布证书;

![](/res/img/blog/tools/IOS/add_certificates.png)

选择App Store and Ad Hoc 后点击 `continue`

![](/res/img/blog/tools/IOS/add_certificates2.png)

再次 `continue`

![](/res/img/blog/tools/IOS/add_certificates3.png)

选择`CSR` 文件.这个文件需要到 `MAC` 电脑上用钥匙串进行生成,所以打开钥匙串

点击钥匙串访问 -> 证书助理 ->从证书颁发机构请求证书.

![](/res/img/blog/tools/IOS/yaoshi.png)

电子邮件地址随意填写(任意自己的邮箱地址),邮箱常用名可不填,存储到磁盘.

![](/res/img/blog/tools/IOS/yaoshi2.png)

这样就会生成一个`CSR`文件，现在上传，回到上面打开的网页:

![](/res/img/blog/tools/IOS/add_certificates4.png)

完成后点击 `DownLoad` 下载证书文件

![](/res/img/blog/tools/IOS/add_certificates5.png)

直接双击上面下载下来的证书，这样就可以在钥匙串中看到证书，确保证书有效;

![](/res/img/blog/tools/IOS/add_certificates6.png)



## 3. 创建发布描述文件(Profile)

描述文件用来描述那台电脑能对那个 `Bundle Identifier` 的工程进行打包测试或者发布.

点击 `Provisioning Profiles`,点击All,再点击右上角"+"号.

![](/res/img/blog/tools/IOS/add_profile.png)

- 如果是发布,选择App Store这个描述文件,英译为:创建发布描述文件以提交你的app到App Store;

- 如果仅是安装到不同手机上进行测试,选择Ad Hoc,英译为:创建发布描述文件以安装你的app到已经注册的设备上(注册的设备上限为100台),点击Continue.

![](/res/img/blog/tools/IOS/add_profile2.png)

在App ID这个选项栏里面找到你刚刚创建的:App ID,点击Continue.


![](/res/img/blog/tools/IOS/add_profile3.png)

选择你刚创建的发布证书，根据自己电脑上的发布证书日期来选择，点击Continue.

![](/res/img/blog/tools/IOS/add_profile4.png)

在 `Profile Name` 栏里输入一个名称,这个是 `Provisioning Profile` (简称PP文件)文件的名称,可随便输入,文件名后缀可带上日期,方便以后使用.然后点击 Continue.

![](/res/img/blog/tools/IOS/add_profile5.png)

`Download` 文件,并双击运行,点击done完成.

![](/res/img/blog/tools/IOS/add_profile6.png)


## 4. XCode工程配置

打开工程项目，先编译下，有错误在接着修改:

### 4.1 配置 版本和名称

![](/res/img/blog/tools/IOS/xcode_setting.png)

### 4.2 配置证书

![](/res/img/blog/tools/IOS/xcode_setting2.png)

### 4.3 全部修改为 `Release` 模式

把Run、Test、Profile、Analyze、Archive中的Build Configuration全部改为Release.之后Close.

![](/res/img/blog/tools/IOS/xcode_setting3.png)

### 4.4 Archive 打包

编译成功,选择Product -> Archive.进行打包.

![](/res/img/blog/tools/IOS/xcode_setting4.png)

### 4.5 工程签名和证书详细配置

>发布时只关心和配置 `Release`的就行了

![](/res/img/blog/tools/IOS/xcode_setting5.png)

![](/res/img/blog/tools/IOS/xcode_setting6.png)

![](/res/img/blog/tools/IOS/xcode_setting7.png)


## 5. 上架配置

![](/res/img/blog/tools/IOS/appStore.png)


点击左下角 `+`,选择 `IOS App Store`，新增上架iOS App需要的证书(iOS Distribution Certificate)

![](/res/img/blog/tools/IOS/appStore2.png)



## 6. 参考

- [简书-惜乐](https://www.jianshu.com/p/440ea5a2bb54)
- [segmentfault-zhuqian](https://segmentfault.com/a/1190000009577706)
- [Appcoda-台湾](https://www.appcoda.com.tw/ios-app-submission/)

## 7. 其他

- 关于SKU

`SKU` 是公司用来统计数据之类的id,根据需要进行填写，能区分不同的app就行，如果出错会给你提醒；


******

    作者:鹅卵石
    时间:2018年5月23日21:57:53
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="/res/img/myCode.png" width="80%" height="80%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---
