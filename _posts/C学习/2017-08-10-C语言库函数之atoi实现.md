---
layout : post
title: C语言库函数之atoi实现
category : C学习
wangyiyun: true
date : 2017-08-10
---

******

    作者:devstone(微信公众号devstone)
    时间:2017年08月10日13:47:11
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->


## C语言库函数之atoi实现

库函数原型:

```C
头文件:
#include <stdlib.h>
int atoi(const char *nptr);
```

- 功能
将字符串转换成整形数

自己实现这个库函数需要考虑以下情况:
- 指针为NULL判断；
- 空字符处理；
- 正负号的处理；
- 溢出处理；
- 遇到异常字符怎么处理；

具体实现代码

**myatoi.h**
```C
#ifdef __cplusplus
extern "C"{

#endif

	int my_atoi(const char *nptr);

#ifdef __cplusplus
}
#endif

```

**myatoi.c**
```C
int my_atoi(const char *nptr)
{
	int total = 0;
	int flag = 1;

	//排除NULL
	if (nptr == NULL)
		return 0;

	//排除空格
	while (*nptr == ' ')
		nptr++;

	//正负号处理
	if ( (*nptr == '-') || (*nptr == '+') )
	{
		if (*nptr == '-')
			flag = -1;
		nptr++;
	}

	//溢出和异常字符过滤处理
	while ((*nptr != '\0') && (*nptr >= '0') && (*nptr <= '9'))
	{
		//计算
		total = total * 10 + *nptr - '0';
		//对溢出进行判断处理
		if (total > INT_MAX || total < INT_MIN)
			return 0;
		nptr++;
	}

	return total * flag;
}
```

测试

```C
	char str[] = "-12345678";

	printf("num:%d\n",my_atoi(str));
```

![](/res/img/blog/C学习/myatoi.png)

```C
	char str[] = "-123a45678";

	printf("num:%d\n",my_atoi(str));
```
![](/res/img/blog/C学习/myatoi2.png)

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)
