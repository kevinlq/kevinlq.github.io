---
layout: page
title: Life
description: 人越学越觉得自己无知
keywords: 生活, Life
comments: false
menu: 生活
permalink: /life/index
---

> 生活中的点点滴滴

<ul class="listing">
{% for life in site.life %}
{% if life.title != "Life Template" %}
<li class="listing-item">
<a href="{{ life.url }}">{{ life.title }}</a>
</li>
{% endfor %}
</ul>
