---
layout : post
title: Qt笔记之容器操作
category : Qt学习
wangyiyun: true
date : 2017-06-11
---

******

    作者:鹅卵石
    时间:2017年6月11日10:07:17
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

## Qt笔记之容器操作
整理下之前在使用容器时遇到的一些问题，以及注意事项

### 容器分类
Qt中的容器和标准的STL容器类似，具体在使用过程中有一些区别

Qt提供的容器更轻量、更安全、更容易使用，而且提供了隐式数据共享、不可变的特性，为速度做了优化，局域较低的内存占用量，最重要的一点是它是线程安全的。这些容器类时平台无关的，不会因编译器不同而具有不同的实现。

隐式数据共享，也叫做"**写时复制(copy on write)**",在我们使用容器传递参数时妈不会出现额外的性能损失



1. QList(QLinkedList)
2. QVector
3. QMap
4. QHash
5. QStack
6. QQueue
7. ……


* **QList<T>** 是基于数组实现的列表，提供了快速的向前添加和向后追加的操作，中间插入数据性能没有QLinkedList好;
* **QLinkedList<T>** 是基于链表实现的，在中间插入大量的数据时性能要优于QList;
* **Qvector** 用于在内存的连续存储区区域存放给定类型的值，在头部或者中间插入数据可能会很慢，因为这回引起大量数据在内存中移动;
* **QMap<key,T>** 提供了字典结构类型，通常每个键与一个值关联，一般以键的顺序进行存储，如果顺序无关，QHash提供了更好的性能;
* **QMultiMap<key,T>** 是QMp子类，提供了多值映射，一个键可以与多个值进行关联;
* **QStack<T>** 是QVector的子类，提供了后进先出( **LIFO** );
* **QQueue<T>** 是QList的子类，提供了先进先出( **FIFO** );
* **QHash<key,T>** 和QMap具有类似的接口，但是提供了更快的查找，QHash以字母顺序存储数据;
* **QMultiHash<key,T>** 这是QHash子类，提供了多值散列;


### 容器迭代

#### Java风格

java风格迭代器是在Qt4首先引入的，属于Qt应用程序推荐使用的形式；使用起来方便，但是没有STL方式高效

![java风格迭代器](/res/img/blog/Qt学习/container_java.png)

java 风格迭代器指向的是两个元素之间的位置，而不是元素本身，因此，可能会指向第一个元素之前的位置，也可能指向集合的最后元素之后的位置；

每一种容器都有两种Java风格的迭代器：**只读**访问和**读写**访问

|容器 | 只读遍历 | 读写遍历|
|---|---|---|
|QList  &lt; T &gt; <br>QQueue&lt; T &gt; |QListIterator&lt; T &gt; | QMutableListIterator&lt; T &gt; |
|QLinkedList&lt; T &gt;|QLinkedListIterator&lt; T &gt;|QMutableLinkedListIterator&lt; T &gt;|
QVector&lt; T &gt;<br>QStack&lt; T &gt; |QVectorIterator&lt; T &gt; |QMutableVectorIterator &lt; T &gt;|
|QSet &lt; T &gt; |WSetIterator&lt; T &gt; |QMutableSetIterator&lt; T &gt;|
|QMap &lt;key, T &gt;<br>QMultiMap &lt;key, T &gt; |QMapIterator &lt; T &gt;| QMutableMapIterator &lt; T &gt;|
|QHash &lt; key,T &gt;<br> QMultiHash &lt; key,T &gt; |QHashIterator &lt; T &gt;| QMutableHashIterator &lt; T &gt;|

**QList简单使用**
```C++
 QList<fileRecord> m_recordList;
     bool status = false;
    //加载数据
    for (int i = 0;i < 10;i++)
    {
        FileRecord record;
        record.bChecked = status;
        record.strFilePath = QString("E:/Qt/file/pic/image_%1.png").arg(i);
        record.strFileSize = QString("%1KB").arg (i*10);
        record.strSpeed = QString("%1KB/s").arg (i);
        m_recordList.append (record);
    }
```

```C++
    QListIterator<fileRecord> i(m_recordList);
    while (i.hasNext ()){
        qDebug ()<<i.next ().strFilePath;
    }
```

需要修改元素时，使用QMutableListIterator
```C++
    QMutableListIterator<fileRecord> iter(m_recordList);
    while(iter.hasNext ())
    {
        if (iter.next ().bChecked){
            iter.remove ();
        }
    }
```
当时在对list操作时，没有注意到只读，对容器进行动态删除后发现删不干净。原因是删除某个元素后，list的索引发生的变化

#### STL风格

STL风格的迭代器从Qt2.0就开始提供，并对速度进行了优化.STL风格迭代器直接指向元素本身

![STL风格迭代器](/res/img/blog/Qt学习/container_stl.png)


有两种STL风格的容器遍历器，一种是只读，一种是读写访问。推荐使用只读访问，因为速度比读写访问要快

|容器 | 只读遍历 | 读写遍历|
|---|---|---|
|QList &lt; T &gt;<br>QQueue &lt; T &gt;|QList &lt; T &gt;::const_iterator |QList &lt; T &gt;::iterator |
|QLinkedList &lt; T &gt; |QLinkedList &lt; T &gt;::const_iterator |QLinkedList &lt; T &gt;::iterator |
|QVector &lt; T &gt;<br>QStack &lt; T &gt; |QVector &lt; T &gt;::const_iterator |QVector &lt; T &gt;::iterator|
|QSet &lt; T &gt; |QSet &lt; T &gt;::const_iterator |QSet &lt; T &gt;::iterator |
|QMap &lt;key, T &gt;<br>QMultiMap &lt;key, T &gt;|QMap &lt;key, T &gt;::const_iterator |QMap &lt;key, T &gt;::iterator|
|QHash &lt;key, T &gt;<br>QMultiHash &lt;key, T &gt; | QHash &lt;key, T &gt;::const_iterator |QHash &lt;key, T &gt;::iterator|

Qlist相关代码
```C++
    QList<fileRecord>::iterator i;
    for (i = m_recordList.begin ();i != m_recordList.end ();++i){
        //*i = (*i).strFilePath;
        qDebug ()<<(*i).strFilePath;
    }
```

debug

>"E:/Qt/file/pic/image_0.png" 
"E:/Qt/file/pic/image_1.png" 
"E:/Qt/file/pic/image_2.png" 
"E:/Qt/file/pic/image_3.png" 
"E:/Qt/file/pic/image_4.png" 
"E:/Qt/file/pic/image_5.png" 
"E:/Qt/file/pic/image_6.png" 
"E:/Qt/file/pic/image_7.png" 
"E:/Qt/file/pic/image_8.png" 
"E:/Qt/file/pic/image_9.png"

因为list中存储的是一个结构体类型，所以这里只打印出了其中一个成员变量的值

### foreach关键字

一般开发中，当仅仅只是遍历一下容器内的元素，可以使用Qt的关键字foreach,比如
```C++
    foreach (fileRecord cord, m_recordList) {
        if (!cord.bChecked){
            ui->checkBox->setChecked (false);
            return;
        }
        ui->checkBox->setChecked (true);
    }
```

代码中的结构体
```C++
typedef struct FileRecord{
    bool bChecked;          //是否选中
    QString strFilePath;    //文件路径
    QString strFileSize;    //文件大小
    QString strSpeed;       //传输速度
} fileRecord;
```

### 注意点
* 对容器进行动态删除时，要考虑到删除一个元素后，容器的索引发生了变化；
* 根据实际需求，选择合适的容器；
* 只是读取容器元素，就不要使用读写容器方式进行遍历；

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)