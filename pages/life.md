---
layout: page
title: Life
description: 人越学越觉得自己无知
keywords: 生活, Life
comments: true
menu: 生活
permalink: /life/
---

> 生活中的点点滴滴

<ul class="listing">
{% for life in site.life %}

<li class="listing-item"><a href="{{ life.url }}">{{ life.date | date:"%Y-%m-%d" }}-{{ life.title }}</a>
</li>
{% endfor %}
</ul>
