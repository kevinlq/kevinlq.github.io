---
layout: page
title: Life
description: 人越学越觉得自己无知
keywords: Life, track，record
comments: false
menu: Life
permalink: /my-life/
---

> 生活中的碎碎念……

<ul class="listing">
{% for my-life in site.my-life %}
{% if my-life.title != "my-life Template" %}
<li class="listing-item"><a href="{{ my-life.url }}">{{ my-life.title }}</a></li>
{% endif %}
{% endfor %}
</ul>
