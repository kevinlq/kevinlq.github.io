---
layout : post
title: Qt笔记之常见换肤样式表代码
category : Qt学习
wangyiyun: true
date : 2017-05-29
---

******

    作者:鹅卵石
    时间:2017年5月29日10:04:01
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

## Qt笔记之常见换肤样式表代码
整理下之前一直使用过的样式表部分

### 1. QTableWidget
#### 处理如下：
* tab标签没有背景图片，无边框；
* tab标签添加了3个伪状态颜色显示，按下时是深绿色，正常状态绿色，鼠标滑过浅绿色，使用rgba进行控制透明度。当按下某个tab时，其他tab的状态低4个像素

```css
QTableWidget:pane{
    border-width:0px;
    border-color:white;
    border-style:outset;
    border-raidus:3px;
    background-color:rgb(132,171,208);
    background:transparent;
}

QTableWidget::tab-bar{
    left:0px;
}
QTabBar::tab{
    width: 160px;
    height:46px;
    font:16px;
    color:white;
	background:#2ABFA3;
}
QTabBar::tab:hover{
    color:white;
	background:rgba(26, 188, 156, 120);
}
QTabBar::tab:selected{
    margin-left:1;
    margin-right:0;
    color:white;
	background:#16A085;
}

QTabBar::tab:!selected{
    color:white;
    margin-left:1;
    margin-right:0;
}
QTabBar::tab:!selected{
    margin-top:4px;
    margin-bottom:-4px;
}
```
#### 样式说明
___
* pane指整个tab组件；
* border-width:指组件的外边框宽度；
* bordre-color:外边框颜色;
* border-style:外边框风格;
* border-radius:外边框圆角像素；

#### 效果图
![](/res/img/blog/Qt学习/2017-05-29_10-16-21-tablewidget-style.png)

#### 没有背景的样式

```css
QTabWidget::pane{
	border:none;
}
QTabWidget::tab-bar{
        alignment:left;
}
QTabBar::tab{
	background:transparent;
	color:rgb(0,0,0);
	min-width:30ex;
	min-height:10ex;
}
QTabBar::tab:hover{
	background:rgb(255, 255, 255, 100);
}
QTabBar::tab:selected{
	border-color: white;
	background:white;
	color:green;
}
```
![](/res/img/blog/Qt学习/2017-05-29_10-16-21-tablewidget-style2.png)


---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)