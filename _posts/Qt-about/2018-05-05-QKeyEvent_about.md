---
layout: post
title: Qt keyEvent事件问题
categories: Qt学习
description: Qt keyEvent事件问题
keywords: QKeyEvent
---
今天一个朋友问了一个关于Qt 按键事件的问题，当时觉得没有问题，自己动手试了下果然有问题，看来以前没有怎么好好看这块!
特意查看了下原因，做个记录!

## 问题描述
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

## 处理方法

官方文档中关于这两个事件是这样说的:

```
	This event handler, for event \a event, can be reimplemented in a
    subclass to receive key press events for the widget.

    A widget must call setFocusPolicy() to accept focus initially and
    have focus in order to receive a key press event.

    If you reimplement this handler, it is very important that you
    call the base class implementation if you do not act upon the key.
	
	也就是说，该方法可以在子类中重载从而使子类可以处理键盘key press event。
	但是，窗口部件必须拥有焦点才可以接收键盘事件
	
	If you reimplement this handler, it is very important that you call the
	base class implementation if you do not act upon the key.
	如果我们对于派生类重载了keyPressEvent()，注意如果对不需要处理的键值需要调用父类的事件处理函数。
	否则这些键值可能得不到处理
```

源码实现方法:

```
void QWidget::keyPressEvent(QKeyEvent *event)
{
    if ((windowType() == Qt::Popup) && event->matches(QKeySequence::Cancel)) {
        event->accept();
        close();
    } else {
        event->ignore();
    }
}
```

最终解决方案

```
void MWidget::keyPressEvent(QKeyEvent *event)
{
    if ( Qt::Key_W ==  event->key () && !event->isAutoRepeat ())
    {
        qDebug()<<"w keyPressEvent..";
    }
}

void MWidget::keyReleaseEvent(QKeyEvent *event)
{
    if ( Qt::Key_W ==  event->key () && !event->isAutoRepeat ())
    {
        qDebug()<<"w keyReleaseEvent...";
    }
}
```

上面的`isAutoRepeat()`方法很重要，


******

    作者:鹅卵石
    时间:2018年4月30日14:15:51
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
