---
layout: post
title: QML Canvas之尴尬的小鱼
categories: [QML学习]
description: QML Canvas绘制一条小鱼
keywords: QML,Canvas
---

以前对 QML绘图不了解，抽时间学习了下使用`Canvas`绘图，发现还是挺便捷的.参考简书上某位`android`牛人实现的小金鱼，跟着思路模仿了一个大概.

[小鱼绘制原理](https://www.jianshu.com/p/3dd3d1524851)

>上面的连接中原理讲解的比较详细，各种公式+示意图，真的很棒，希望自己以后也能写的更认真些.


## 效果

<center>
<img src="/res/img/blog/QML-learn/Canvas/canvas_fish.gif" width="50%" height="50%" />
</center>

## 核心
```
Canvas{
    id: canvas
    anchors.fill: parent
    onPaint: {
        var ctx = getContext("2d")
        ctx.reset();
        //set m_otherAlpha
        ctx.fillStyle = 'rgba(244, 92, 71, 0.6)';
        drawBody(ctx,m_headRadius);

        //set m_otherAlpha
        ctx.fillStyle = 'rgba(244, 92, 71, 0.61)';
    }
```

>通过绘制小鱼，发现好多数学基本公式确实还给老师了，今天慢慢找回了一些，实在惭愧惭愧......

源码等完善后再放出，可执行程序在这里[密码：rga6](https://pan.baidu.com/s/1nvpCptR)

## 草稿

小鱼是在是复杂，画个图好理解些.........

<center>
<img src="/res/img/blog/QML-learn/Canvas/fish_draf.jpg" width="40%" height="40%" />
</center>

画的实在是太丑，没有一点艺术细胞...........

## 参考文章

[小鱼绘制原理l](https://www.jianshu.com/p/3dd3d1524851)

---

******

    作者:鹅卵石
    时间:2017年12月24日22:11:18
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。

<!-- more -->

**如果您对本文有任何问题，可以在下方留言，或者Email我.**
