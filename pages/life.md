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

<ul class="listing">

{% for life in site.life %}
* [{{ life.name }}]({{ life.url }})
{% endfor %}

</ul>
