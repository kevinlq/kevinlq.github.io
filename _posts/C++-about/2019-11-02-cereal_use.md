---
layout: post
title: C++ 序列化库 Cereal使用(一)
categories: [C++]
description: C++ 序列化库 Cereal使用
keywords: C++, cereal
---


## cereal 介绍

`Cereal` 是一个head only开源的C++11序列化库，支持自定义数据类型序列化成`JSON`,`XML`,`二进制压缩`,反之也可以反序列化。几乎不依赖于其他第三方库(RapidJson,RadidXml)，方便快速。


## 下载
由于是开源项目，直接在`Github`上可以下载最新版本.

![github](https://gitee.com/devstone/imageBed/raw/master/blog/C++/cereal/2019-11-02_23-12-05.png)

```
git clone git@github.com:USCiLab/cereal.git

```

## 使用

### 配置

将上一步下载下来的库包含到自己的工程路径中来，比如:

```
INCLUDEPATH +=$$PWD/Cereal
```

### 定义数据类型

定义自己的数据类型，编写序列化函数，以下是我自己代码中测试的类:

```
class QDicomTag
{
public:
    QDicomTag();
    ~QDicomTag();

    /*! 自定义序列化函数*/
    template <class Archive>
    friend void serialize( Archive & ar, const QDicomTag &tag, std::uint32_t const &version);

public:
    void cleanTag();

    bool loadTag(DcmDataset *pDataSet);
    bool loadTagGroup0008(DcmDataset *pDataSet);
    bool loadTagGroup0010(DcmDataset *pDataSet);
    bool loadTagGroup0020(DcmDataset *pDataSet);
    bool loadTagGroup0028(DcmDataset *pDataSet);

    std::string m_strImageType;                         // (0008,0008)
    std::string m_strSopClassUID;                       // (0008,0016)
    std::string m_strSopInstanceUID;                    // (0008,0018)
    std::string m_strStudyDate;                         // (0008,0020)
    std::string m_strSeriesDate;                        // (0008,0021)
}；

template<class Archive>
void serialize(Archive &ar, const QDicomTag &tag, std::uint32_t const &version)
{
    (void)version;

    ar(cereal::make_nvp("imageType", tag.m_strImageType)
       ,cereal::make_nvp("sopClassUID", tag.m_strSopClassUID)
       ,cereal::make_nvp("sopInstanceUID", tag.m_strSopInstanceUID)
       );
}

```

关键部分是编写序列化函数，具体序列化由序列化函数来实现，我这里为了代码整洁，序列化函数放在了类的外部,具体可以看后面的序列化函数.


### 开始序列化

需要包含头文件

```
#include "cereal/macros.hpp"
#include "cereal/specialize.hpp"
#include "cereal/version.hpp"
#include "cereal/access.hpp"
#include "cereal/cereal.hpp"

#include "cereal/archives/binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/archives/json.hpp"

#include "cereal/types/memory.hpp"
#include "cereal/types/string.hpp"
#include <cereal/types/vector.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/map.hpp>
```

```
{
	QString strFile = "D:/dev/TestSerialization.json";
    std::ofstream os(strFile.toStdString ());
    cereal::JSONOutputArchive archive(os);

    QDicomTag tag;
    // ....这里省去tag结构的初始化工作

    archive(cereal::make_nvp("dicomTag" ,tag));	
}
```


## 序列化函数

序列化函数可以分为以下几种，具体类的结构如上面定义的`DicomTag`

### 定义在类的内部

1. 使用单个序列化函数

```
template<class Archive>
void serialize(Archive &ar)
{
	ar(m_strImageType, m_strSopClassUID, m_strStudyDate, m_strSeriesDate);
}
```

可以根据自己需要，选择序列化的字段，序列化函数中列出的项均会被序列化，上面的写法，在序列化输出后会自动添加`Value0.....`字段值，为了方便可以添加自己的字段名：

```
template<class Archive>
void serialize(Archive &ar)
{
	    ar(cereal::make_nvp("imageType", tag.m_strImageType)
       ,cereal::make_nvp("sopClassUID", tag.m_strSopClassUID)
       ,cereal::make_nvp("sopInstanceUID", tag.m_strSopInstanceUID)
       );
}
```

具体输出结果

- XML

```
<?xml version="1.0" encoding="utf-8"?>
<cereal>
	<dicomTag>
		<imageType>DERIVED</imageType>
		<sopClassUID>1.2.840.10008.5.1.4.1.1.7</sopClassUID>
		<sopInstanceUID>1.2.156.14702.1.1005.128.2.201607141416245011010000001</sopInstanceUID>
	</dicomTag>
</cereal>
```

- JSON

```
{
    "dicomTag": {
        "imageType": "DERIVED",
        "sopClassUID": "1.2.840.10008.5.1.4.1.1.7",
        "sopInstanceUID": "1.2.156.14702.1.1005.128.2.201607141416245011010000001",
    }
}
```

2. 分来save和load函数

```
template<class Archive>
void save(Archive &ar) const
{
		ar(cereal::make_nvp("imageType", tag.m_strImageType)
       ,cereal::make_nvp("sopClassUID", tag.m_strSopClassUID)
       ,cereal::make_nvp("sopInstanceUID", tag.m_strSopInstanceUID)
       );
}
```

```
template<class Archive>
void load(Archive &ar) const
{
		ar(cereal::make_nvp("imageType", tag.m_strImageType)
       ,cereal::make_nvp("sopClassUID", tag.m_strSopClassUID)
       ,cereal::make_nvp("sopInstanceUID", tag.m_strSopInstanceUID)
       );
}
```

上述函数，输出后结果和1相同.


### 定义在类的外部

1. 使用单个序列化函数

```
class QDicomTag
{
public:
    QDicomTag();
    ~QDicomTag();

    /*! 自定义序列化函数*/
    template <class Archive>
    friend void serialize( Archive & ar, const QDicomTag &tag, std::uint32_t const &version);
}；

template<class Archive>
void serialize(Archive &ar, const QDicomTag &tag, std::uint32_t const &version)
{
    (void)version;

    ar(cereal::make_nvp("imageType", tag.m_strImageType)
       ,cereal::make_nvp("sopClassUID", tag.m_strSopClassUID)
       ,cereal::make_nvp("sopInstanceUID", tag.m_strSopInstanceUID)
       );
}

```

2. 分来save和load函数

```
class QDicomTag
{
public:
    QDicomTag();
    ~QDicomTag();

    /*! 自定义序列化函数*/
    template <class Archive>
    friend void save(Archive &a, const QDicomTag &tag, std::uint32_t const &version);

    template <class Archive>
    friend void load(Archive &a, const QDicomTag &tag, std::uint32_t const &version);
}；

template<class Archive>
void save(Archive &ar, const QDicomTag &tag, std::uint32_t const &version)
{
    (void)version;

    ar(cereal::make_nvp("imageType", tag.m_strImageType)
       ,cereal::make_nvp("sopClassUID", tag.m_strSopClassUID)
       ,cereal::make_nvp("sopInstanceUID", tag.m_strSopInstanceUID)
       );
}

template<class Archive>
void load(Archive &ar, const QDicomTag &tag, std::uint32_t const &version)
{
    (void)version;

    ar(cereal::make_nvp("imageType", tag.m_strImageType)
       ,cereal::make_nvp("sopClassUID", tag.m_strSopClassUID)
       ,cereal::make_nvp("sopInstanceUID", tag.m_strSopInstanceUID)
       );
}

```


## 其他

## 参考文章

- [cereal官方文档] [1]


[1]: http://uscilab.github.io/cereal/index.html

******

    作者:鹅卵石
    时间: 2019年11月2日22:33:08
    版本:V 0.0.1
    邮箱:kevinlq@163.com
    微信公众号:devstone
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="/res/img/myCode.png" width="50%" height="50%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---
