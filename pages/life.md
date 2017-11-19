---
layout: page
title: Life
description: 人越学越觉得自己无知
keywords: 生活, Life
comments: false
menu: 生活
permalink: /life/
---

> 生活中的点点滴滴


<section class="container posts-content">
{% assign sorted_life = site.life | sort %}
{% for life in sorted_life %}
<h3>{{ life | first }}</h3>
<ol class="lifes-list" id="{{ life[0] }}">
{% for post in category.last %}
<li class="lifes-list-item">
<span class="lifes-list-meta">{{ life.date | date:"%Y-%m-%d" }}</span>
<a class="lifes-list-name" href="{{ life.url }}">{{ life.title }}</a>
</li>
{% endfor %}
</ol>
{% endfor %}
</section>
