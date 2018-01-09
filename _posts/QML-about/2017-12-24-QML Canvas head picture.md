---
layout: post
title: QML Canvas之绘制圆形头像
categories: [QML学习]
description: QML Canvas绘制圆形头像
keywords: QML,Canvas,clip
---

学习了`Canvas`基础知识后，越来越觉得很强大，也很方便，实现某个效果简简单单几行代码就可以搞定，突然想到了圆形头像这个常见的效果，平时使用的软件中很多场景可以看到。

## 效果

<center>
<img src="/res/img/blog/QML-learn/Canvas/canvas_headPicture.png" width="50%" height="50%" />
</center>

动态缩放效果，实际使用时应该设定大小!
<center>
<img src="/res/img/blog/QML-learn/Canvas/canvas_head_show.gif" width="50%" height="50%" />
</center>

## 原理

使用Canvas 绘制一个圆形区域，然后对画布进行裁剪，使用`clip`，之后直接使用drawImage进行绘制即可，需要注意的是计算好各自的坐标位置即可.

## 核心代码

```qml
    ctx.arc(cx, cy, r, 0, 2 * Math.PI,true);
    ctx.clip();
    ctx.drawImage(m_picturePath, x, y, d, d);

    drawWhiteBorder(ctx,x,y,r);
```

绘制最外边白圈
```
    function drawWhiteBorder(ctx,x,y,r)
    {
        ctx.strokeStyle = "#FFFFFF"
        ctx.lineWidth = 4;
        ctx.arc( x,y,r,0,Math.PI*2);
        ctx.stroke();
    }
```


调用
```
    onPaint:
    {
        var ctx = canvas.getContext("2d");
        ctx.reset();
        toRoundBitmap(ctx);
    }
```

## 参考文章

[Canvas基础教程](https://developer.mozilla.org/zh-CN/docs/Web/API/Canvas_API)


---

******

    作者:鹅卵石
    时间:2018年1月7日21:30:49
    版本:V 0.0.2
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。

<!-- more -->

**如果您对本文有任何问题，可以在下方留言，或者Email我.**
