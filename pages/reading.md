---
layout: page
title: Reading
description: 读书使人进步
keywords: 阅读, Reading
comments: false
menu: 阅读
permalink: /reading/
---

```
2017年11月创建了这个tab，计划以后一些读书笔记记录在这里，算是一种纪念吧.
```

> 阅读使人进步

<ul class="listing">
{% for read in site.reading %}
{% if read.title != "read Template" %}
<li class="listing-item"><a href="{{ read.url }}">{{ read.date | date:"%Y-%m-%d" }}-{{ read.title }}</a>
</li>
{% endif %}
{% endfor %}
</ul>
