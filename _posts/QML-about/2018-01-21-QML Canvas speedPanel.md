---
layout: post
title: QML Canvas之绘制仪表盘大揭秘
categories: [QML学习]
description: QML Canvas绘制仪表盘
keywords: QML,Canvas,speed,panel
---

不得不说canvas确实很方便快捷。在绘制2D图形这快简直太方便，以前用QWidget绘制过，今天没事就想想用QML来绘制。


## 效果

先来看看绘制后的一个简单效果:
<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/1.png" width="80%" height="80%" />
</center>

## 绘制步骤

1. 创建画布
2. 绘制中心圆
3. 绘制文字(单位以及数字)
4. 绘制刻度
5. 绘制指针
6. 绘制最外圈大圆
7. 添加update接口，进行动态刷新显示

>以上步骤中其中几项可以合并在一起的，但是为了清晰所以单独列出来进行绘制.

##难点
绘制仪表盘难点在于刻度角度的计算，以及指针位置的计算。

## 开始绘制
进行绘制时，按照上述步骤挨个绘制即可，需要注意的就是坐标原点，为了便于绘制，开始时直接将画布原点进行了平移，移到了窗口中央。

## 相关数学知识
绘制常见的 2D 图像用的基本数学较多，还给老师的知识必须补补了。

### 三角函数
<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/sanjiao.png" width="80%" height="80%" />
</center>

仪表盘上的刻度起始位置就是圆上的某个点，因此确定点的坐标很重要。
```
sinθ = AC / AO
cosθ = OC / AO
```
AO即为圆的半径，θ的值需要根据刻度间隔来确定。

<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/2.png" width="80%" height="80%" />
</center>

### 圆心方程
```
 (x-a)²+(y-b)²=r²，圆心O(a，b)，半径r。
```
<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/3.jpg" width="80%" height="80%" />
</center>

继而可以求出圆上任意一点的坐标了:
```
圆心:x0,y0
半径:r
角度:angle
圆上任意一点为:(x1,y1)
x1 = x0 + r * cos(angle) 
y1 = y0 + r * sin(angle) 
```

### 创建画布

```QML
    Canvas
    {
        id: canvas
        width: parent.width
        height: parent.height

        onPaint:
        {
            var ctx = getContext("2d");

            drawSpeedPanel(ctx);
            ctx.restore();
        }
    }
```
这样以后的绘制到会在函数`drawSpeedPanel`中进行。

### 移动原点

```QML
    var width = canvas.width
    var height = canvas.height
     //计算每一个刻度占的度数
    var perAngle = 300/((m_speedNum.length - 1) * m_step);

    ctx.clearRect(0,0,width,height);
    ctx.save();
    //将原点移到中心点,方便绘制
    ctx.translate(width/2,height/2);
```
上述代码将坐标原点移到了窗口中心位置，在绘制结束后要恢复，要不然坐标会紊乱.

每次开始重绘之前先清空当前画布，然后再重绘！

### 绘制最中心圆
```QML
    function drawCenterCircle(ctx)
    {
        ctx.beginPath();
        var radial = ctx.createRadialGradient(0,0,m_radius*0.5,0,0,m_radius);
        radial.addColorStop(0,m_colorCenter);
        radial.addColorStop(1,"#6D8CA3");
        ctx.fillStyle = radial;
        ctx.arc(0,0,m_radius,0,Math.PI*2,false);
        ctx.fill();

        ctx.closePath();
    }
```

上面创建了一个渐变填充，绘制圆后会进行填充。

## 原理

### 圆心方程
```
已知圆心(a,b),半径:r,求圆上某一点的坐标(x,y):

(x-a)*(x-a) + (y-b)*(y-b) = r*r

```


## 参考文章

[Canvas基础教程](https://developer.mozilla.org/zh-CN/docs/Web/API/Canvas_API)

---

******

    作者:鹅卵石
    时间: 2018年1月21日9:55:47
    版本:V 0.0.1
    邮箱:kevinlq@yeah.net
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是最知识的尊重。

<!-- more -->

**如果您对本文有任何问题，可以在下方留言，或者Email我.**
