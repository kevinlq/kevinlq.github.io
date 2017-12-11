---
layout : post
title: Qt与百度地图交互(javascript)
category : Qt学习
wangyiyun: true
date : 2017-05-16
---

******

    作者:鹅卵石
    时间:2017年5月16日22:32:45
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

##  Qt与百度地图交互(javascript)

### 前言

Qt和百度地图或者其他地图进行交互，无外乎采用webkit和javascript进行交互，最近做个地图展示工具，有要求离线使用，所以只能下载好离线地图数据，考虑到时间紧就直接使用Qt加载html进行交互显示

### Qt调用javascript中的函数方法

Qt中通过evaluateJavaScript()方法和js之间进行交互

#### 实现目的
通过界面给定经纬度坐标值，直接在地图上生成对应的marker标记

#### 实现方法
```
void Widget::addMarker(const QString &lot, const QString &lat,
                        const QString &micon)
{
    QWebFrame *frame = ui->webView->page()->mainFrame();
    QString marker = QString("addMarker(\"%1\",\"%2\",\"%3\")")
	.arg(lot).arg(lat).arg(micon);
    frame->evaluateJavaScript(marker);
}
```

map.html
```
//添加mark标记
function addMarker(lot,lat,micon){		
	var mkIcon = new BMap.Icon(micon, new BMap.Size(25,25));
	var point = new BMap.Point(lot, lat);
	var mk = new BMap.Marker(point,{icon:mkIcon});   // 创建标注
	map.addOverlay(mk);                              // 加载标注

	//为标记创建信息窗口
	createInfoWindow("aaa","bbb");

	mk.addEventListener("mouseover", function(){          
		this.openInfoWindow(infoWindow); //开启信息窗口
	});
	mk.addEventListener("onmouseout", function(){          
		this.closeInfoWindow(infoWindow); //关闭信息窗口
	});
	mk.addEventListener("click", function(e){          
		ReinforcePC.getCoordinate(e.point.lng,e.point.lat);
	});
};
```

createInfoWindow()方法实现
```
var infoWindow;
function createInfoWindow(shipName,mmsi){
	
	//创建信息窗口内容
	var content = "<table>";
		content = content + "<tr><td> 船名:"+ shipName + "</td></tr>";
		content = content + "<tr><td> MMSI:"+ mmsi + "</td></tr>";
		content = content + "<tr><td> 经度:"+ shipName + "</td></tr>";
		content = content + "<tr><td> 纬度:"+ mmsi + "</td></tr>";
		content +="</table>";
		
	// 创建信息窗口对象 
	infoWindow = new BMap.InfoWindow(content);

};
```

### javascript调用Qt中的方法

#### 实现目的

通过点击地图中的marker返回该marker对应的位置信息，简单的返回经纬度信息

#### 实现方法
先关联webview的一个信号到对应的槽函数中，注意该槽函数必须为共有的
```
    connect(ui->webView->page()->mainFrame(), 
	SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(slotPopulateJavaScriptWindowObject()));
```

```
void Widget::slotPopulateJavaScriptWindowObject()
{
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject(
	"ReinforcePC", this);
}
```

将获取到的经纬信息输出
```
void Widget::getCoordinate(const QString &lot, const QString &lat)
{
    qDebug()<<"lot:"<<lot<<" lat:"<<lat;
}
```

map.html
```
mk.addEventListener("click", function(e){          
	ReinforcePC.getCoordinate(e.point.lng,e.point.lat);
});
```
为marker添加了鼠标点击事件，在点击事件中添加对应方法返回经纬度坐标



### 覆盖物
一般给地图中添加覆盖物很简单，初始化对应覆盖物之后，执行```map.addOverlay(marker);```即可，当然大部分需求是需要对添加的覆盖物进行操作的，比如删除、更新等等，这样就涉及到准确的定位到需要的覆盖物，这里记录标记marker的操作.

#### 添加一个marker

添加marker时需要添加额外的信息，比如label,可以更好的找到该marker
```js
var pointsLen = points.length;
if(pointsLen == 0){
	return;
}
var myIcon = new BMap.Icon("images/mark.png", new BMap.Size(25, 25), {
	offset: new BMap.Size(5, 5) 
});

// 创建标注对象并添加到地图   
for(var i = 0;i <pointsLen;i++){
	var point = new BMap.Point(points[i].lng,points[i].lat);
	var label = new BMap.Label(points[i].name,{offset:new BMap.Size(20,-10)});
	var marker = new BMap.Marker(point, {icon: myIcon});
	marker.setLabel(label);
	map.addOverlay(marker); 
```

在创建marker对象时，可以给对象的ICON设置名称，这样在遍历删除过程中可以采用对应marker的ICON名称进行识别

```js
BMap.Icon.prototype.name = "";
BMap.Icon.prototype.setName = function(name){
      	this.name = name;
      }
```
创建marker时设置名称
```js
  var mkIcon = new BMap.Icon("images/move.png", new BMap.Size(40,40));
  mkIcon.setName(shipName);
```

#### 删除某个marker

这里进行删除标记点采用name进行过滤查找
```js
function deleteMarker(points){
	var len = points.length;
	if (len == 0){
		return;
	}
	var allOverlay = map.getOverlays();
	for (var i = 0;i < allOverlay.length;i++){
		if (allOverlay[i] instanceof BMap.Marker){
			if (allOverlay[i].getLabel.content == name ){
				map.removeOverlay(allOverlay[i]);
				return false;
			}
		}
	}
}
```

当给ICON设置名称后，遍历过程可以这样
```js
if (allOverlay[i].getIcon().name == name){
	map.removeOverlay(allOverlay[i]);
	return false;
}
```

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)