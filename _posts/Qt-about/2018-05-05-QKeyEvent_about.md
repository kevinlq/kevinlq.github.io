---
layout: post
title: QImage源码分析之Save方法实现
categories: Qt学习
description: QImage保存图像分析
keywords: QImage,save
---
今天一个朋友问了一个关于Qt 按键事件的问题，当时觉得没有问题，自己动手试了下果然有问题，看来以前没有怎么好好看这块!
特意查看了下原因，做个记录!

## 问题
重载`keyPressEvent`和`keyReleaseEvent`两个事件，指定一个按键，比如`W`,只要按键按下，就输出对应的信息:

关键代码
```
void MWidget::keyPressEvent(QKeyEvent *event)
{
    if ( Qt::Key_W ==  event->key () )
    {
        qDebug()<<"w keyPressEvent..";
    }
}

void MWidget::keyReleaseEvent(QKeyEvent *event)
{
    if ( Qt::Key_W ==  event->key ())
    {
        qDebug()<<"w keyReleaseEvent...";
    }
}
```

当一直按下W键不放，控制台交替输出`w keyPressEvent..`,`w keyReleaseEvent...`,`w keyPressEvent..`,`w keyReleaseEvent...`

也就是说按下事件和释放事件交替触发了，这就怪了!

结果：

![](/res/img/blog/Qt-learn/KeyEvent/error_result.png)





******

    作者:鹅卵石
    时间:2018年4月30日14:15:51
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

![](/res/img/myCode.png)

如果觉得分享的内容不错，可以请作者喝杯咖啡!
