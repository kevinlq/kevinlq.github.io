---
layout: post
title: Qt on android 启动时出现黑屏问题
categories: [Qt on Android]
description: Qt on android 启动时出现黑屏问题
keywords: Qt,android
---


看到Qt能开发android程序了，很是高兴，特意下载体验了一把，不巧的是中间遇到了一些奇怪的问题，比如黑屏问题，不过还是找到办法可以解决的。

## 解决思路

解决黑屏问题还是得从java入手！

打开android工程目录下：android\res\values\，新建一个styles.xml文件，直接在eclipse中新建即可。

打开编写一下内容：

```xml
<?xml version='1.0' encoding='utf-8'?>  
<resources>  
    <style name="MyApp">  
        <item name="android:windowDisablePreview">true</item>  
        <item name="android:windowBackground">@null</item>  
        <item name="android:windowContentOverlay">@null</item>  
        <item name="android:windowIsTranslucent">false</item>  
        <item name="android:windowAnimationStyle">@null</item>   
    </style>  
</resources>
```
继续打开`AndroidManifest.xml`配置文件，application节点后添加如下内容：

```xml
android:theme="@style/MyApp"
```

一个完整的示例：
```xml
<?xml version='1.0' encoding='utf-8'?>
<manifest package="org.qtproject.example.L_QMLBaseCompoments" xmlns:android="http://schemas.android.com/apk/res/android" android:versionName="1.0" android:versionCode="1" android:installLocation="auto">
    <application android:hardwareAccelerated="true" android:name="org.qtproject.qt5.android.bindings.QtApplication"
        android:icon="@drawable/kevinlq"
        android:theme="@style/devstone"
        android:label="@string/kevinlq_single_app_name">
        <activity android:configChanges="orientation|uiMode|screenLayout|screenSize|smallestScreenSize|layoutDirection|locale|fontScale|keyboard|keyboardHidden|navigation"
                  android:name="org.qtproject.qt5.android.bindings.QtActivity"
                  android:screenOrientation="portrait"
                  android:launchMode="singleInstance">
            <intent-filter>
                <action android:name="android.intent.action.MAIN"/>
                <category android:name="android.intent.category.LAUNCHER"/>
            </intent-filter>
```

## 解释

- 设置app图标
```xml
android:icon="@drawable/kevinlq"
```

- 设置主题
```xml
android:theme="@style/devstone"
```

- 设置app名字
```xml
android:label="@string/kevinlq_single_app_name"
```

- 添加启动loader图片
```xml
<meta-data android:name="android.app.splash_screen_drawable" android:resource="@drawable/loader"/>
```

以上就是一个简单的配置过程，特此做下笔记！


******

    作者:鹅卵石
    时间:2015年04月10日10:16:40
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。




---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**
