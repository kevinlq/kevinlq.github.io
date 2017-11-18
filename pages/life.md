---
layout: page
title: Life
description: 生活点滴
keywords: 生活，Life
comments: true
menu: 生活
permalink: /life/
---

> 生命在不停的流逝

<ul class="listing">
{% assign sorted_lifes = site.life | sort %}
{% for life in sorted_lifes %}
<li class="listing-item"><a href="{{ life.url }}">{{ life.title }}</a>
</li>
{% endfor %}
</ul>

