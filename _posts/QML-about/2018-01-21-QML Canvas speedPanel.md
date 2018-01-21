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
<img src="/res/img/blog/QML-learn/Canvas/speed/speed.gif" width="50%" height="50%" />
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

## 难点
绘制仪表盘难点在于刻度角度的计算，以及指针位置的计算。

## 开始绘制
进行绘制时，按照上述步骤挨个绘制即可，需要注意的就是坐标原点，为了便于绘制，开始时直接将画布原点进行了平移，移到了窗口中央。

## 相关数学知识
绘制常见的 2D 图像用的基本数学较多，还给老师的知识必须补补了。

### 三角函数
<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/sanjiao.png" width="50%" height="50%" />
</center>

仪表盘上的刻度起始位置就是圆上的某个点，因此确定点的坐标很重要。
```
sinθ = AC / AO
cosθ = OC / AO
```
AO即为圆的半径，θ的值需要根据刻度间隔来确定。

<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/2.png" width="50%" height="50%" />
</center>

### 圆心方程
```
 (x-a)²+(y-b)²=r²，圆心O(a，b)，半径r。
```
<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/3.jpg" width="50%" height="50%" />
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

### 绘制单位以及数值

```
//绘制单位以及速度值
function drawUnitText(ctx)
{
    //文字大小根据圆的半径自动调整,这里因为canvas没有设置文本字体大小属性，所以只能用字符串拼接了
    ctx.font = m_radius*1.2+ "px serif"
    ctx.textBaseline = "middle";
    ctx.fillStyle = m_textColor;

    //绘制上面的单位km/h
    ctx.fillText(m_unitName,-m_radius*1.2, -m_radius*2.4);
    //绘制下面的速度80
    ctx.fillText(m_value, -m_radius*0.6,m_radius*2.4);
}
```

为了使字体大小能基本进行适配，所以上面对字体大小做了小的处理。

### 绘制刻度

```QML
//绘制刻度
function drawScale(ctx,valueArry,perAngle)
{
    var i = 0;
    //计算数组长度
    var length = valueArry.length - 1;
    //            ctx.rotate( rads(120) );
    for ( var deg = 0; deg <= length*m_step; deg++)
    {
        ctx.beginPath();
        var angle = -240 + perAngle*deg;
        var spotX = m_scaleRadius*Math.cos(rads(angle))
        var spotY = m_scaleRadius*Math.sin(rads(angle))
        var spot = m_radius *0.36;
        ctx.lineWidth = 1.2;
        if ( deg >= (length - 4)*m_step)
        {
            ctx.fillStyle = "#E6F612"
            ctx.strokeStyle = "#E6F612"
        }
        if ( deg >= (length - 2)*m_step)
        {
            ctx.fillStyle = "#CA0B14"
            ctx.strokeStyle = "#CA0B14"
        }

        if ( 0 == deg %10)
        {
            ctx.lineWidth = 2.2
            ctx.strokeStyle = m_textColor
            spot = m_radius * 0.58
            var textX = m_scaleRadius*1.24*Math.cos(rads(angle));
            var textY = m_scaleRadius*1.24*Math.sin(rads(angle));
            ctx.font = m_radius*0.68+"px Arial";
            ctx.textBaseline = "middle";// 文字垂直对齐方式
            ctx.textAlign = "center";   // 文字水平对齐方式
            ctx.fillText(valueArry[i],textX,textY);
            i++
        }
        if ( (0 == deg%5) && (0 != deg%10))
        {
            ctx.lineWidth = 1.6
            spot = m_radius * 0.48
        }

        var endX = (m_scaleRadius + spot )*Math.cos(rads(angle));
        var endY = (m_scaleRadius + spot )*Math.sin(rads(angle));

        ctx.moveTo(spotX,spotY);
        ctx.lineTo(endX,endY);
        ctx.stroke();
        ctx.closePath();
    }
}
```
绘制刻度属于中重点，主要是计算了绘制的坐标！因为不是360度进行绘制，只绘制300度，所以需要进行计算。

<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/4.png" width="50%" height="50%" />
</center>

### 绘制指针
```QML
//画指针
function drawPointer(ctx,curValue,perAngle)
{
    //计算当前的value所处的角度
    var transAngle = curValue*perAngle;
    console.log("transAngle:"+transAngle)
    console.log("perAngle:"+perAngle)
    ctx.save();
    ctx.beginPath();
    ctx.rotate(rads(120 + transAngle));
    //            ctx.globalAlpha = 0.5
    var gradient = ctx.createRadialGradient(0, 0, m_radius, 0, 0, m_pointerRadius*1.2);
    gradient.addColorStop(0, 'rgba(16,110,180,0.6)');
    gradient.addColorStop(0.5, 'rgba(21,165,174,0.8)');
    gradient.addColorStop(1,'rgba(30,130,139,1)');
    ctx.fillStyle = gradient;

    //设定指针夹角大小(这里是角度)，这里可以根据需要进行调整
    var angle = 40;
    //计算圆上点的坐标
    var point1 = getCircleCoordinate(m_radius,0,0,angle);
    var point2 = [m_pointerRadius,0];
    ctx.moveTo(point1[0],point1[1]);
    ctx.arc(0,0,m_radius,rads(angle),rads(-angle));
    ctx.lineTo(point2[0],point2[1]);
    ctx.lineTo(point1[0],point1[1]);
    ctx.fill();
    ctx.closePath();
    ctx.restore();
}
```
这里就用到了计算圆上任意点的坐标了

```
已知圆心(a,b),半径:r,求圆上某一点的坐标(x,y):

(x-a)*(x-a) + (y-b)*(y-b) = r*r

```

```QML
/*
  求圆上任意一点的坐标
  已知圆的半径，圆心坐标，以及对应的角度
*/
function getCircleCoordinate(radius,x,y,angle)
{
    var point=[];
    var potX = x + radius * Math.cos( rads(angle));
    var potY = y + radius * Math.sin( rads (angle));

    point.push(potX);
    point.push(potY);

    return point;
}
```

### 绘制最外圈大圆
```QML
    //画最外面的大圆
    function drawOutCircle(ctx)
    {
        ctx.beginPath();
        var radial = ctx.createRadialGradient(0,0,m_radius*7,0,0,m_radius*8.2);
        radial.addColorStop(0,"#244461");
        radial.addColorStop(0.5,"#265882");
        radial.addColorStop(1,"#2D628A");
        ctx.fillStyle = radial;
        ctx.arc(0,0,m_radius*7,0,Math.PI*2,false);
        ctx.arc(0,0,m_radius*8.2,Math.PI*2,false);
        ctx.fill();

        ctx.closePath();
    }
```
最外圈大圆其实就是绘制了2个同心圆，然后进行渐变填充。效果不是很好，还可以继续美化处理。

最终的效果就是这个样子了:

<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/1.png" width="50%" height="50%" />
</center>

## 测试
使用android套件编译下，手机上的效果如下:
<center>
<img src="/res/img/blog/QML-learn/Canvas/speed/5.jpg" width="50%" height="50%" />
</center>

目前没有做分辨率适配，很明显，刻度数字太大了，完了得添加适配进去.

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
