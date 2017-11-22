---
layout: page
title: Reading
description: 读书使人进步
keywords: 阅读, Reading
comments: false
menu: 阅读
permalink: /reading/
---

> 阅读使人进步

<ul class="listing">
{% for read in site.reading %}
{% if read.title != "read Template" %}
<li class="listing-item"><a href="{{ read.url }}">{{ read.title }}</a>
</li>
{% endif %}
{% endfor %}
</ul>
