---
layout : life
title: mysql TIMESTAMPDIFF
category : 数据库学习
duoshuo: true
date : 2015-01-22
---

<!-- more -->


## **TIMESTAMPDIFF**

***语法:***

```mysql
	TIMESTAMPDIFF(interval,datetime_expr1,datetime_expr2)
```

说明:

返回日期或者日期时间的表达式datetime_expr1和datetime_expr2之间的整数差,期结果的单位由interval参数给出,该参数必须时以下值的其中一个:

* 1. FRAC_SECOND 毫秒
* 2. SECOND 秒
* 3. MINUTE 分钟
* 4. HOUR 小时
* 5. DAY 天
* 6. WEEK 星期
* 7. MONTH 月
* 8. QUARTER 季度
* 9. YEAR 年

## **TIMESTAMPADD**

***语法：***

```mysql
	TIMESTAMPADD(interval,int_expr,datetime_expr)
```

说明：
将整型表达式inter_expr天价到日期或者日期时间表达式datetime＿expr中，式中的interval和上下文中列举的取值时一样的结果


