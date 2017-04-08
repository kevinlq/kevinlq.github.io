---
layout : life
title: InnoSetup提升系统管理员权限
category : 项目工程管理
duoshuo: true
date : 2014-11-26
---

<!-- more -->

******

在```[SetUp]```段添加：

>
```
PrivilegesRequired=admin
```

找到```INNO```安装目录下的```SetupLdr.e32```文件(其实就是一个exe程序),将程序中的```Manifest```内的
 
>
``` <requestedExecutionLevel level="asInvoker" uiAccess="false"/></requestedPrivileges> ```

改为 

>
```<requestedExecutionLevel level="requireAdministrator" uiAccess="false"/></requestedPrivileges> ```

此处需要一个工具才能修改：Resource Hacker

**应用程序运行时申请管理员权限，如下图所示即可：**

![示意图](/res/img/blog/程序打包/2014-04-04_151006.png)

**或者在pro中添加：**

>
```
QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"
```
