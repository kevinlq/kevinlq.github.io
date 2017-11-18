---
layout: page
title: Life
description: 生活点滴
keywords: 生活，life
comments: true
menu: 生活
permalink: /life/index
---

> 生命在不停的流逝

<ul class="listing">
{% for life in site.life %}
{% if life.title != "life Template" %}
<li class="listing-item"><a href="{{ life.url }}">{{ life.title }}</a>
</li>
{% endif %}
{% endfor %}
</ul>
