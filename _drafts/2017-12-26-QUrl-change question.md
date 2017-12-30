---
layout: post
title: QUrl 自动将大小转成小写问题
categories: [Qt学习]
description: QUrl 会自动将大写的url内容转成小写
keywords: QUrl
---

在使用QUrl 进行打开一个URL操作时，发现会自动将我的URL内容转成小写，导致在接收方出现问题.

```
QString strParm = "HTTP://ABC";
qDebug()<<strParm;

QUrl url(strParm);

```

debug info:
```
"HTTP://ABC"
QUrl("http://abc")
```



******

    作者:鹅卵石
    时间:2017年12月26日18:18:19
    版本:V 0.0.2
    邮箱:kevinlq@yeah.net

<!-- more -->

Content here

---
>本博客的文章标有 **鹅卵石** 标签都属于作者原创文章，欢迎大家转载分享。
但是，希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。

**如果您对本文有任何问题，可以在下方留言，或者Email我.**