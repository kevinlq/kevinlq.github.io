---
layout : post
title: C/C++柔性数组(可变长数组)
category : C学习
wangyiyun: true
date : 2017-08-16
---

******

    作者:devstone(微信公众号devstone)
    时间:2017年08月16日13:47:11
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->


## C/C++柔性数组(可边长数组)


### 定义

数组大小可变(待定)的数组

>C99标准之后引入了柔性数组的概念。在这之前，我们需要在结构体中存放一个动态长度的
字符串，一般都是定义一个char类型指针，使用该指针指向动态内存空间地址。但是这样操作不太方便，
而且定义的指针也会占用结构体内存啊！

### 简单的柔性数组

```C++
typedef struct myStruct{
	int length;
	char array[0];	
}st,*pst;

int main(int argc,char *argvp[] )
{ 
    printf("myStruct:%d\n", sizeof(myStruct));
    system("pause");
    return 0;
}
```

输出结果:

![柔性数组大小](/res/img/blog/C学习/2017-08-17_15-59-58.png)

可以看到大小为4，只有成员变量length大大小，array不占用结构体的空间。

柔性数组反映了C语言对精炼代码的极致追求，这种结构对于通信中的变长数据包结构非常实用。



### 动态分配大小

```C++
typedef struct myStruct{
	int length;
	char array[0];	
}st,*pst;

int main(int argc,char *argvp[] )
{ 
    printf("myStruct:%d\n", sizeof(myStruct));

    char c1[] = "I am short string test!";
    char c2[] = "I am long string test!";

    pst p = (pst)malloc(sizeof(st) + strlen(c1) + 1);
    if (NULL != p)
    {
    	p->length = 10;
    	strcpy(p->array,c1);
    }

    printf("p address:%p\n", &p);
    printf("p->length:%d\n", p->length);
    printf("p->array:%s\n", p->array);

	printf("myStruct:%d\n\n", sizeof(myStruct));

	pst p2 = (pst)malloc(sizeof(st) + strlen(c2) + 1);
    if (NULL != p)
    {
    	p2->length = 11;
    	strcpy(p2->array,c2);
    }

    printf("p2 address:%p\n", &p2);
    printf("p2->length add:%p\n", &p2->length);
    printf("p2->array add:%p\n", &p2->array);
    printf("p2->length:%d\n", p2->length);
    printf("p2->array:%s\n", p2->array);

    free(p);
    free(p2);

    system("pause");
    return 0;
}

```

输出结果:

![](/res/img/blog/C学习/2017-08-17_16-18-59.png)

说明:

- 可以根据需要进行动态分配结构体大小；
- 任意时刻，对结构体求sizeof,均为4；
- 打印出结构体成员地址可以看出，array和length差4字节(刚好为一个结构体大小)


### 使用指针和使用变长结构体的区别

1. 在位置方面：指针可以放在任何地方，**但是变长结构体的变长部分一定要放在结构体的最后;**
2. 字内存方面：指针会占用一个指针大小的内存空间，但是变长结构体不会，它只是一个占位符;
3. 在内存布局方面：指针指向的结构体内存可以是不连续的，但是变长结构体和结构体的内存必然是连续的；
4. 内存释放方面：使用指针时，需要先释放指针所指的内存空间再释放整个结构体，否则会造成内存泄漏；
5. 一个限制：指针可以使用在C++的类中，但是变长结构体就不可以了。因为某些编译器会将一些额外的
信息放在类的最后，比如vptr或者虚基类的内容，使用变长的类时就会把这部分的值改变，这种行为时未定义的，
没有人知道会发生什么！


>**注意：柔性数组必须为结构体的最后一个成员！！！**

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)
