---
layout: post
title: Qt 对象序列化/反序列化
categories: [Qt]
description: 序列化对象
keywords: Qt, C++
---

> 阅读本文大概需要 3 分钟

## 背景

日常开发过程中，避免不了对象序列化和反序列化，如果你使用 `Qt` 进行开发，那么有一种方法实现起来非常简单和容易。

## 实现

我们知道 `Qt`的元对象系统非常强大，基于此属性我们可以实现对象的序列化和反序列化操作。

比如有一个学生类，包含以下几个字段：学号、姓名、性别、出生日期等，定义如下类结构：

```C++
class DStudent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString number READ number WRITE setNumber)
    Q_PROPERTY(QString sex READ sex WRITE setSex)
    Q_PROPERTY(QDateTime birthday READ birthday WRITE setBirthda)
public:
    explicit DStudent(QObject *parent = nullptr);
	
	QString name() const;
    void setName(const QString &newName);
	
    QString number() const;
    void setNumber(const QString &newNumber);

    QString sex() const;
    void setSex(const QString &newSex);
	
	QDateTime birthday() const;
    void setBirthda(const QDateTime &newBirthday);
	
	//...
```

需要增加那些字段，只需要在上述属性位置继续追加即可，通过把需要反射的字段定义成属性，我们就可以遍历该类的元对象，进而获取其中的属性信息。

### 序列化 Json

```C++
QJsonObject DStudent::toJson()
{
    QJsonObject jsObj = KJsonHelp::object2Json(this);
    return jsObj;
}
bool DStudent::fromJson(const QJsonObject &jsObj)
{
    return KJsonHelp::json2Object(jsObj, this);
}
```

核心代码封装到工具类中，方便其它地方调用，详细实现如下：

```C++
QJsonObject KJsonHelp::object2Json(QObject *object)
{
    QJsonObject jsObj;
    if(nullptr == object)
    {
        return jsObj;
    }

    const QMetaObject *pMetaObj = object->metaObject();
    for(int i = 0; i < pMetaObj->propertyCount(); i++)
    {
        auto proName = pMetaObj->property(i).name();
        jsObj.insert(proName, QJsonValue::fromVariant(object->property(proName)));
    }

    if(jsObj.contains("objectName"))
    {
        jsObj.remove("objectName");
    }

    return jsObj;
}

bool KJsonHelp::json2Object(const QJsonObject &jsObj, QObject *object)
{
    if (jsObj.isEmpty() || nullptr == object)
    {
        return false;
    }

    QStringList list;
    const QMetaObject *pMetaObj = object->metaObject();
    for(int i = 0; i < pMetaObj->propertyCount(); i++)
    {
        list << pMetaObj->property(i).name();
    }
    QStringList jsonKeys = jsObj.keys();
    foreach(const QString &proName ,list)
    {
        if(!jsonKeys.contains(proName) || jsObj.value(proName).isNull())
        {
            continue;
        }
        object->setProperty(proName.toLocal8Bit().data(), jsObj.value(proName));
    }

    return true;
}
```

任意一个继承 `QObject`的对象都可以获取到它的元对象，接着可以获取到属性个数，然后挨个进行遍历即可。

如果想序列化到其他格式的，比如XML，在上述方法中根据 `XML` 规则生成即可，这个不是本文的重点。

## 一些坑和注意点

当然了并不是所有的类型都支持这种方式自动生成字段的，一些特殊类型或者自定义的类需要自己特殊去实现。

我们可以在上述学生类中，继续添加新的测试属性字段，来看看输出的结果：
```C++
    // custome type
    Q_PROPERTY(DScore sScore READ sScore WRITE setSScore)

    // test other type
    Q_PROPERTY(int testInt READ testInt WRITE setTestInt)
    Q_PROPERTY(bool testBool READ testBool WRITE setTestBool)
    Q_PROPERTY(double testDouble READ testDouble WRITE setTestDouble)
    Q_PROPERTY(char testChar READ testChar WRITE setTestChar)
    Q_PROPERTY(QUrl testUrl READ testUrl WRITE setTestUrl)
    Q_PROPERTY(QVariant testV READ testV WRITE setTestV)
    Q_PROPERTY(QStringList testStringList READ testStringList WRITE setTestStringList)
    Q_PROPERTY(QRect testRect READ testRect WRITE setTestRect)
    Q_PROPERTY(QSize testSize READ testSize WRITE setTestSize)
    Q_PROPERTY(QPoint testPoint READ testPoint WRITE setTestPoint)
    Q_PROPERTY(QList<int> testIntList READ testIntList WRITE setTestIntList)
    Q_PROPERTY(QList<QString> testListString READ testListString WRITE setTestListString)
```

打印输出：
```C++
    qRegisterMetaType<DScore>("DScore");
    DStudent st;
    st.setName(QStringLiteral("法外狂徒张三"));
    st.setNumber("123456789");
    st.setSex(QStringLiteral("男"));
    st.setBirthda(QDateTime::currentDateTime());

    // test other type
    st.setTestInt(10);
    st.setTestBool(true);
    st.setTestV(12);
    st.setTestDouble(12.121212);
    st.setTestChar('k');        //->string
    st.setTestUrl(QUrl("http://kevinlq.com/")); // -> string
    st.setTestStringList(QStringList() << "stringList1" << "stringList2");
    st.setTestRect(QRect(10,10,10,10));             // null
    st.setTestSize(QSize(10,10));                   // null
    st.setTestPoint(QPoint(10, 10));                // null
    st.setTestIntList({11, 12});                    // null
    st.setTestListString({"kevinlq", "devstone"});  // null

    DScore score;
    score.setName("computer");
    score.setNumber("001");
    st.setSScore(score);                            // null

    qDebug() << "st:" << st.toJson();
```

```
st: QJsonObject({"birthday":"2023-08-05T19:33:14.815","name":"法外狂徒张三","number":"123456789","sScore":null,"sex":"男","testBool":true,"testChar":"k","testDouble":12.121212,"testInt":10,"testIntList":null,"testListString":null,"testPoint":null,"testRect":null,"testSize":null,"testStringList":["stringList1","stringList2"],"testUrl":"http://kevinlq.com/","testV":12})

```
可以看到很多字段的值是 `null`，出现这种问题表示这个类型目前无法直接自动生成，如果你缺失需要这种结构，那么需要自行在序列化函数中进行特殊处理，比如自定义的类型处理:

```
QJsonObject DStudent::toJson()
{
    QJsonObject jsObj = KJsonHelp::object2Json(this);
    jsObj.insert("sScore", m_sScore.toJson());
    return jsObj;
}
```
上述对定义的课程类，进行了特殊处理,再次编译后，输出的结果如下：
```
{
	"birthday": "2023-08-05T23:27:00.757",
	"name": "法外狂徒张三",
	"number": "123456789",
	"sScore": {
		"name": "computer",
		"number": "001"
	}
}
```
可以看到正常输出了我们需要的类型，其他类型可以照葫芦画瓢。

## 进阶
是不是发现问题了，随着你的类属性字段越来越多，手写这么多字段肯定非常累，能否自动生成这些重复的代码呢，答案是可以的，结合前面的文章，就可以编写一份更加紧凑的代码了，详细见这里 http://kevinlq.com/2023/01/16/generateProperty/

## 总结
序列化其实有很应用场景，以下是工作站经常使用的小 case：

- 持久化保存类对象，及保存数据到本地磁盘；
- socket 传输数据，需要把json/xml/other类型转为对象，用对象进行业务处理；
- 数据库操作：从 db 中读取出来的值序列化成对象，方便业务进行处理(使用 ORM 框架例外)
- 和界面交互，比如界面使用 QML 编写，那么大部分超场景会使用到 JSON。
- 其他：待补充……


## 参考文档

- [c++自动生成get/set方法](http://kevinlq.com/2023/01/16/generateProperty/)


******

    作者:鹅卵石
    时间: 2023年8月5日23:41:31
    版本:V 0.0.1
    邮箱:kevinlq@163.com
    版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
    希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->

---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**