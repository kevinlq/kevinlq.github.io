---
layout : post
title: C++实现单链表
category : C++学习
wangyiyun: true
date : 2014-05-20
---

******

    作者:鹅卵石
    时间:2014年05月20日20:38:09
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

# C++实现单链表
学习完链表就用C++实现了一个简答的链表，作为练手

[源码在这里](http://download.csdn.NET/detail/u013704336/7376653)

link.h

```
#ifndef LINK_H_INCLUDED  
#define LINK_H_INCLUDED  
#include <iostream>  
#include <stdlib.h>  
using namespace std;  
  
class Link  
{  
public:  
    Link();  
    ~Link();  
    Link* CreateNOde(int data);//创建一个结点  
    void Free(Link *p);//释放结点指针  
    void InsertNode(Link *p);//插入新的结点  
    void Traverse();//遍历链表  
    Link *Find(int data);//查找某一个结点的值  
    int DeleteNode(Link *p);//删除结点  
    void Destroy();//销毁链表  
private:  
    int data;//链表的data域  
    Link * next;//指向下一结点的指针  
    Link *head;//头指针；  
};    
#endif // LINK_H_INCLUDED
```

link.cpp

```
#include "link.h"  
  
Link::Link()//对类的成员进行初始化  
{  
    next = 0;  
    head = NULL;  
}  
  
Link::~Link()//析构函数的实现  
{  
    delete next;  
    delete head;  
}  
  
Link* Link::CreateNOde(int data)//创建一个结点  
{  
    Link *p = new Link;  
    p->data = data;  
    return p;  
}  
  
void Link::Free(Link *p)//释放结点,要加头文件#include <stdlib.h>  
{  
    free(p);  
}  
  
void Link::InsertNode(Link *p)//插入结点  
{  
    p->next = head;  
    head = p;  
}  
  
void Link::Traverse()   //遍历链表  
{  
    Link *p = head;     //p指向了头结点  
    while(p != NULL)    //一直到p为空而终止  
    {  
        cout << p->data;   //输出每个结点的值  
        p = p->next;  
    }  
    cout <<endl;  
}  
  
Link * Link::Find(int data)     //查找某一个值是否是该结点  
{  
    Link *p = head;             //p指向了头结点  
  
    while(p != NULL)            //一直到p为空而终止  
    {  
        if(p->data == data)  
        {  
            return p;  
        }  
        p = p->next;  
    }  
    return NULL;  
}  
  
int Link::DeleteNode(Link *p)       //删除某一个结点  
{  
    if(p == head)                   //头结点的情况，直接使其指向下一个结点  
    {  
        head = head->next;  
        return 1;  
    }  
  
    Link *pre = head;           //定义一个指向其前驱结点的指针  
    while(pre->next != NULL)  
    {  
        if(pre->next == p)  
        {  
            pre->next = p->next;  
            return 1;  
        }  
        pre = pre->next;  
    }  
  
    return 0;  
}  
  
void Link::Destroy()        //销毁链表  
{  
    Link *p;  
    while(head != NULL)  
    {  
        p = head;  
        head = head->next;  
        Free(p);  
    }  
}
```

main.cpp

```
/*DAta:2014/5/19  
  Autho:LQ  
  Function：用C++实现一个简单的单链表  
  采用头插法进行插入结点，然后进行遍历输出。  
  */  
#include <iostream>  
#include "link.h"  
using namespace std;  
  
int main()  
{  
    Link l;                         //构建一个对象l,以下通过l调用类的成员函数  
    Link *p = l.CreateNOde(1);      //创建一个结点，其数据域为1  
    l.InsertNode(p);                //  将该结点插入到链表中来  
    l.Traverse();                   //遍历进行显示到屏幕上  
  
    p = l.CreateNOde(2);  
    l.InsertNode(p);  
    l.Traverse();  
  
    p = l.CreateNOde(3);  
    l.InsertNode(p);  
    l.Traverse();  
  
    l.Destroy();    //销毁链表的操作  
    l.Traverse();   //再次遍历，得到的结果显示没有数据了。  
    return 0;  
} 
```

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)


