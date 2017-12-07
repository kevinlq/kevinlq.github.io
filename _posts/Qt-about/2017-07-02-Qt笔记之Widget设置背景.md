---
layout : post
title: Qt笔记之Widget设置背景
category : Qt学习
wangyiyun: true
date : 2017-07-02
---

******

    作者:鹅卵石
    时间:2017年7月2日9:43:54
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

## Qt笔记之Widget设置背景

整理下之前给widget设置背景常见的方法

### 方法
Qt中给widget设置背景图片有三种方法，各有利弊

#### 1. 使用样式表进行设置
使用样式表进行设置时，如果使用QtDesinger进行设置，直接鼠标右键设置样式表，在弹出的对话框中选择资源图片，此时可以选择background-image或者border-image，前者是按像素进行显示，后者根据窗体大小进行缩放，一般选择后者，但是如果图片的大小和widget的大小不一致时会造成图片拼接显示现象，最好自贴图时选择大小一致的图片。

如下图所示

![desinger设计](/res/img/blog/Qt学习/widget_back1.png)

设置完成的效果

![效果](/res/img/blog/Qt学习/widget_back2.png)

---
**注意**
>
在给widget设置背景图片后会发现它上面的子控件也会显示背景图片，这样的效果不是我们想要的，这时可以在该Widget上面放置一个新的widget或者frame，所有的控件都转移到该新的widget(frame)上来，设置样式时，直接给新的widget设置即可.

---

#### 2. 使用QPalette进行绘制

一般在比较复杂的软件设计中，使用纯手工代码创建界面会比较容易控制，这个时候可以使用QPalette来进行绘制

```C++
void BaseWidget::init()
{
    QPalette palette;
    QPixmap pixmap(":/image/home.jpg");
    palette.setBrush (QPalette::Window,QBrush(pixmap));
    this->setAutoFillBackground (true);
    this->setPalette (palette);
}
```
效果图

![效果](/res/img/blog/Qt学习/widget_back3.png)

#### 3. 重写paintEvent()事件

这种实现的效果和第二种方法是一样的

```C++
void BaseWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap (0,0,width(),height (),QPixmap(":/image/home.jpg"));
}
```

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)