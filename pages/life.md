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

# 生命在不停的流逝

******
> 生命流逝的证明(日日当精进 但求无愧)

> **来者轻点生命戳**

<ul class="listing">
{% for life in site.life %}

<li class="listing-item"><a href="{{ life.url }}">{{ life.title }}</a>
</li>

{% endfor %}

</ul>
