---
layout: page
title: Life
description: 人越学越觉得自己无知
keywords: 生活, Life
comments: false
menu: Life
permalink: /life/
---

> 记多少命令和快捷键会让脑袋爆炸呢？

<ul class="listing">
{% for life in site.life %}
{% if life.title != "Life Template" %}
<li class="listing-item"><a href="{{ life.url }}">{{ life.title }}</a>
</li>
{% endif %}
{% endfor %}
</ul>
