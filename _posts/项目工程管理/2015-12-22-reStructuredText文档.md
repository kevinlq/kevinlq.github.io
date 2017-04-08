---
layout : life
title:  reStructuredText 的基本语法
category : 项目工程管理
duoshuo: true
date : 2015-12-22
---

reStructuredText [官方网址](http://docutils.sourceforge.net/rst.html), 打不开吧!!!!!! fuck [GFW](https://zh.wikipedia.org/wiki/%E9%98%B2%E7%81%AB%E9%95%BF%E5%9F%8E)

<!-- more -->

[这里](http://sphinx-doc-zh.readthedocs.org/en/latest/rest.html)是Sphinx中关于`reStructuredText`的翻译，本文也是从里面挑拣自己平时回经常用到的语法进行示例化记录。

**以下内容纯属虚构，图片也是随手在知乎上巴拉的头像**

### 1.提示类信息

```

.. note::

    我是note 信息

.. attention::

    我是attention信息

.. caution::

    我是caution信息

.. warning::

    我是warning信息

.. tip::

    我是tip信息

.. important::

    我是important信息

.. hint::

    我是hint信息

.. error::

    我是error信息

.. danger::

    我是danger信息
```


### 2.标题类信息

在reST中标题非常简单，只要在标题上(下)使用非字母和数字的七位元ASCII记录下标线（或上标线）即可，你不需担心标题会用什么格式，只要在相同位置使用相同标记即可。可用的字符有：! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ ] ^ _ ` { | } ~。


```

============
文章标题测试
============

***************
章节标题测试
***************

小节标题测试
===============

子节标题测试
------------------

子子节标题测试
^^^^^^^^^^^^^^^^^^^^^

段落标题测试
""""""""""""""""""

段落标题测试
******************

```

### 3.字体类信息

```

**加黑字体**

*意大利斜体*

\ 转义字体(后面跟着一个空格意味着后面是一个空格)


``原始形态的字体而非Docutils处理后的文字，通常以等宽字体出现 Hello nami!``

```

### 4.段落 

在reST中，段落只要在中间隔开一行即可，Docutils会自动判别各个段落，例如：

### 5.代码块

```

``I am code``

我是代码测试快::

    # coding: utf-8

    __author__ = 'daodaoliang'

    from flask import Flask
    from flask_restful import Resource, Api

    app = Flask(__name__)
    api = Api(app)


    # noinspection PyMethodMayBeStatic
    class Api_HelloWorld_V1(Resource):
        def get(self):
            return {'hello': 'world'}


    api.add_resource(Api_HelloWorld_V1, '/')

    if __name__ == '__main__':
        app.run(debug=True)

```

### 6. 表格

```

---------------------

+------------------------+------------+----------+----------+
| Header row, column 1   | Header 2   | Header 3 | Header 4 |
| (header rows optional) |            |          |          |
+========================+============+==========+==========+
| body row 1, column 1   | column 2   | column 3 | column 4 |
+------------------------+------------+----------+----------+
| body row 2             | ...        | ...      |          |
+------------------------+------------+----------+----------+

或者更为简洁的方式

=====  =====  ======
   Inputs     Output
------------  ------
  A      B    A or B
=====  =====  ======
False  False  False
True   False  True
False  True   True
True   True   True
=====  =====  ======

```

### 7. 超链接 

+ 在后面加上下划线，只要在指定目标即可形成超链接

```

`我的博客啦啦啦啦`_.

.. _我的博客啦啦啦啦: http://daodaoliang.github.io(这里不仅仅可以时网址，也可以时内部标题，实现内部的跳转) 

```

+ 内嵌网址的形式( `链接显示文字 <网址>`_ )

```
`我的博客啦 <http://daodaoliang.github.io>`_.
```

+ 直接写网址

```
daodaoliang.github.io
```

+ 无名超链接

```
`我是一个很长长长长长长长……的文字`__我是后面的文字

.. __: daodaoliang.github.io

或者更简洁的:

__ daodaoliang.github.io`
```

+ 内部链接

```
`内部标题`_ 
```

+ 间接链接

```
`内部链接`_

.. _内部链接: daodaoliang.github.io

`我是链接`_

.. _我是链接: 内部链接_

```

+ 无内容链接

```
.. _回页首:

`回页首`_
```

### 8. 上下标

```

1. 上标:

123:sup:`2` 意思123的上标为2

2. 下标:

123:sub:`2` 意思123的下表为2

```


### 9. 图片资源

+ 段落间插入图片

```
.. image:: https://pic4.zhimg.com/6a97b4a68a4db3f1d3ae66cd1638965f.jpeg
	:scale: 100
	:height: 120
	:width: 160
	:align: left
	:alt: I'm a girl.
	:target: https://pic4.zhimg.com/6a97b4a68a4db3f1d3ae66cd1638965f.jpeg
   	
	这个女孩我也不认识，只是在知乎随手巴拉的啦，哈哈哈哈(这个功能需要PIL库)
```

+ 文字间插入图片

```

这里有\|内部图片|\，哈哈哈哈哈

.. |内部图片| image:: https://pic4.zhimg.com/6a97b4a68a4db3f1d3ae66cd1638965f.jpeg

```


![这里示例图01](/res/img/blog/项目工程管理/2015-12-22-01.png)
![这里示例图02](/res/img/blog/项目工程管理/2015-12-22-02.png)
![这里示例图03](/res/img/blog/项目工程管理/2015-12-22-03.png)




