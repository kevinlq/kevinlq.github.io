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

在这之前，以及混迹于CSDN[u013704336](http://blog.csdn.net/u013704336).

2017年3月30搬家到了GitHub上，落户于此的目的是为了更好的享受编写博客的乐趣.

--文档即博客(编写一个文档一次commit即可看到效果),另一个比较重要的是可以更加专心，避免了各种突如其来的广告打扰


**目前从事医疗影像相关软件开发**


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
