---
layout: post
title: Qt/C++ 自动生成get和set方法以及属性
categories: [Qt]
description: 使用宏定义来自动生成繁琐的代码
keywords: Qt, C++
---

> 阅读本文大概需要 3 分钟

## 背景

日常开发过程中，避免不了写很多大量的成员变量，如果这些变量都定义成 `public`，那么完全不用写对应的 `get/set` 方法，但是很多时候不能也不允许这样做，当变量/属性很多时手写会变得非常麻烦。

一般在使用 `Qt` 开发时还会增加相关的属性改变信号等属性，当变量值改变后信号会触发，这些重复、繁琐的工作希望能进一步一起简化，使得我们可以从繁琐的工作中解放出来。

基于以上两点，我们可以宏定义此过程中的实现，使用时一行代码搞定

## 实现

在开始动手前可以考虑下，宏定义如何实现变量名拼接、变量命名如何标准化(驼峰法，当然了其它命名规范也可以，主要做到全文保持一致即可)

### 基本语法
```C++
#xx: 参数格式化， 也就是会把传入的参数名转化后参数名字符串
##name： 连接操作， 会把前后两个入参变为一个字符串

示例如下：
#define Function(arg) #arg
输入：std::string str = Function(hello)
输出： 宏展开后就会变成 std::string str = "hello"

#define Function(arg1, arg2) m_##arg
输入: Function(name)
输出：m_name
```

### 小试牛刀
```C++
#define AUTO_PROPERTY(type, typeName ,variableName) \
    Q_PROPERTY(type variableName READ get##variableName WRITE set##variableName NOTIFY s##variableName##Changed ) \
    public: \
    inline type get##variableName() const { return m_##typeName##variableName ; } \
    inline void set##variableName(const type& value) { \
    if (m_##typeName##variableName == value)  return; \
    m_##typeName##variableName = value; \
    emit s##variableName##Changed(value); \
    } \
    Q_SIGNAL void s##variableName##Changed(const type &value);\
    private: \
    type m_##typeName##variableName;
```

### 调用

```C++
// TestClass
class TestClass : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, s, StuName)
    READONLY_PROPERTY(QString, s, StuInfo)
public:
    explicit TestClass(QObject *parent = nullptr);
};

// main.cpp
TestClass t;
t.setStuName("admin");
qDebug() << t.getStuName() << t.getStuInfo();
```

可以看到，大大简化了代码，整体代码量也少很多，可以把重心放在重要的业务和算法中。

但是，没有任何事情是完美的，虽然这样简化了代码，但是有人会说了，代码可读性等是否会降低？到底会不会呢？还是要因人而异吧，你说呢？

## 总结

上述代码是在 `gist.github` 上某个版本基础上修改而成的，主要处理了命名规范，详细可以参考下文连接。

参考这个代码，可以实现其它版本的功能，比如只读变量、更改变量的访问权限等等，详细内容可以见 `GitHub`


## 参考文档

- [firecat全宏](https://juejin.cn/post/6963596491017420808)
- [gist.github](https://gist.github.com/Rolias/48d453a0490d36090193)
- 


******

    作者:鹅卵石
    时间: 2023年1月16日21:24:01
    版本:V 0.0.1
    邮箱:kevinlq@163.com
    版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
    希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->

---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**