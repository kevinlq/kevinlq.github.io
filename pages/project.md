---
layout: page
title: project
description: 我的一些小项目
keywords: 项目
comments: true
menu: 项目
permalink: /project/
---
> 这里是我之前做过的一些小项目

<ul class="listing">
{% for pro in site.project %}
<li class="listing-item"><a href="{{ pro.url }}">{{ pro.date | date:"%Y-%m-%d" }}-{{ pro.title }}</a>
</li>
{% endfor %}
</ul>
