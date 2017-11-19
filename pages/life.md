---
layout: page
title: Life
description: 生活点滴
keywords: 生活，Life
comments: false
menu: 生活
permalink: /life/
---

> 生命在不停的流逝,珍惜

<ul class="listing">
{% for life in site.life %}
{% if life.title != "Life Template" %}
<li class="listing-item"><a href="{{ life.url }}">{{ life.title }}</a>
</li>
{% endif %}
{% endfor %}
</ul>

