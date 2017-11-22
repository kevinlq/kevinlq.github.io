---
layout: page
title: About Me
description: 鹅卵石
keywords: 鹅卵石
comments: true
menu: 关于
permalink: /about/
---


## 说明

在这之前，以及混迹于 CSDN [http://blog.csdn.net/u013704336](http://blog.csdn.net/u013704336).

2017 年 3 月 30 搬家到了 GitHub 上，落户于此的目的是为了更好的享受编写博客的乐趣.

---

## 个人介绍
毕业于一所普通二本院校，网络工程专业。热衷于windows、linux下的应用程序开发。喜欢`C/C++`, `Qt`。
曾近搞过 `C#`，`java`，`STM32`，`zigbee`，画过 `PCB`，焊过电路板。

热爱骑行，在校期间多次骑车远行.

**目前从事医疗影像相关软件开发**

![myself](/res/img/aboutme/mysqlf.jpg)

---

## 联系

{% for website in site.data.social %}
* {{ website.sitename }}：[@{{ website.name }}]({{ website.url }})
{% endfor %}

## Skill Keywords

{% for category in site.data.skills %}
### {{ category.name }}
<div class="btn-inline">
{% for keyword in category.keywords %}
<button class="btn btn-outline" type="button">{{ keyword }}</button>
{% endfor %}
</div>
{% endfor %}

## 友情链接

{% for link in site.data.links %}
* [{{ link.name }}]({{ link.url }})
{% endfor %}
