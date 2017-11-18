---
layout: page
title: Life
description: 生活点滴
keywords: 生活，life
comments: true
menu: 生活
permalink: /life/
---

> 生命在不停的流逝

<section class="container lifes-content">
{% assign sorted_life = site.life | sort %}
{% for life in sorted_life %}
<h3>{{ life | first }}</h3>
<ol class="lifes-list" id="{{ life[0] }}">
{% for post in life.last %}
<li class="lifes-list-item">
<span class="lifes-list-meta">{{ life.date | date:"%Y-%m-%d" }}</span>
<a class="lifes-list-name" href="{{ life.url }}">{{ life.title }}</a>
</li>
{% endfor %}
</ol>
{% endfor %}
</section>
