---
layout: post
title: DCMTK Tag
categories: [医学影像]
description: DCMTK
keywords: DCMTK
---

要解析显示一张 `dcm`图像，首先必须要解析其部分标签信息。

标签信心主要有三类，`Meta info`,普通tag，像素tag，根据需要进行读取

使用 `DCMKT`读取 `dicom`标签是很容易的一件事情

## meata info tag读取

meta 信息在 `0002`组

有三种方式读取，这里展示最简单的一种

```
    DcmMetaInfo metainfo;
    OFCondition status;
    status = metainfo.loadFile("G:/dicomFile/3.dcm");

    if (status.good())
    {
        OFString sopClassUID, xferUID;
        if (metainfo.findAndGetOFString(DCM_MediaStorageSOPClassUID, sopClassUID).good())
            COUT << "SOP Class UID: " << sopClassUID << OFendl;
        if (metainfo.findAndGetOFString(DCM_TransferSyntaxUID, xferUID).good())
            COUT << "Transfer Syntax UID: " << xferUID << OFendl;

        metainfo.print(COUT);
    }
```

输出结果信息：
```
SOP Class UID: 1.2.840.10008.5.1.4.1.1.2
Transfer Syntax UID: 1.2.840.10008.1.2.4.51
# Dicom-Meta-Information-Header
# Used TransferSyntax: Little Endian Explicit
(0002,0000) UL 202                                      #   4, 1 FileMetaInformationGroupLength
(0002,0001) OB 00\01                                    #   2, 1 FileMetaInformationVersion
(0002,0002) UI =CTImageStorage                          #  26, 1 MediaStorageSOPClassUID
(0002,0003) UI [1.2.276.0.7230010.3.1.4.2012715.17328.1511764633.841627] #  56, 1 MediaStorageSOPInstanceUID
(0002,0010) UI =JPEGExtended:Process2+4                 #  22, 1 TransferSyntaxUID
(0002,0012) UI [1.2.276.0.7230010.3.0.3.6.1]            #  28, 1 ImplementationClassUID
(0002,0013) SH [OFFIS_DCMTK_361]                        #  16, 1 ImplementationVersionName
```

[官方参考资料]("https://support.dcmtk.org/redmine/projects/dcmtk/wiki/Howto_LoadMetaHeader"。官方参考资料)

## 普通 tag读取

下面以读取病人姓名为例

```
    DcmFileFormat dcmFormat;

    OFCondition ofResult = dcmFormat.loadFile("G:/dicomFile/3.dcm");

    if (!ofResult.good())
    {
        qDebug() << "dcmFormat read fail..." << ofResult.text();
        return false;
    }

    DcmDataset *pDataSet = dcmFormat.getDataset();

    if (nullptr == pDataSet)
    {
        return false;
    }

    OFString strPatientName;
    ofResult = pDataSet->findAndGetOFString(DCM_PatientName, strPatientName);

    if (!ofResult.good())
    {
        return false;
    }

    qDebug() << "patientName:" << strPatientName.c_str();
```

结果输出
```
patientName: Zhang xxx
```

实际开发当中解析时需要分别对应不同类型，单独封装对应的解析方法

比如解析字符串tag

```
bool getStringTagValue(DcmDataset* pDataSet,const DcmTag& pTag, QString &value)
{
    if (nullptr == pDataSet)
    {
        return false;
    }

    const char* strValue = nullptr;
    Uint32 lLength = 0;
    OFCondition ofResult = pDataSet->findAndGetString(pTag, strValue, lLength);

    if (!ofResult.good())
    {
        return false;
    }

    value = QLatin1String(strValue, lLength);

    return true;
}
```

比如解析浮点数tag

```
bool getU16TagValue(DcmDataset* pDataSet,const DcmTag& pTag, quint16 &nValue)
{
    Uint16	uint16 = 0;

    if ( pDataSet->findAndGetUint16( pTag, uint16).good() )
    {
        nValue = uint16;

        return true;
    }

    return false;
}
```

其实 `DcmFileFormat`对象内部就包含了`DcmMetaInfo`和`DcmDataset`对象指针，在开始的构造函数中内部自动创建的，内部采用了双链表结构进行存储

```
DcmFileFormat::DcmFileFormat()
  : DcmSequenceOfItems(InternalUseTag),
    FileReadMode(ERM_autoDetect)
{
    DcmMetaInfo *MetaInfo = new DcmMetaInfo();
    DcmSequenceOfItems::itemList->insert(MetaInfo);
    MetaInfo->setParent(this);

    DcmDataset *Dataset = new DcmDataset();
    DcmSequenceOfItems::itemList->insert(Dataset);
    Dataset->setParent(this);
}
```


## 总结

当然了，上述方法均是采用 `DCMTK`开源库进行读取，自己造轮子也能一步一步读取，就是比较麻烦一点

`GitHub`上面有一个开源很久的项目 `qdcm`，也是基于`Qt`实现的，可以参考下

https://github.com/Archie3d/qdcm

比如读取文件，判断是否包含头信息代码

```
bool DcmFile::hasHeader(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QDataStream ds(&file);
    int s = ds.skipRawData(128);
    if (s != 128) {
        file.close();
        return false;
    }

    char signature[4];
    s = ds.readRawData(signature, 4);
    if (s != 4) {
        file.close();
        return false;
    }

    file.close();

    if (signature[0] == 'D' && signature[1] == 'I' && signature[2] == 'C' && signature[3] == 'M') {
        return true;
    }

    return false;
}
```

## 参考文章

- [qdcm](https://github.com/Archie3d/qdcm)
- [dcmtk](https://support.dcmtk.org/redmine/projects/dcmtk/wiki/Howto_LoadMetaHeader)

******

    作者:鹅卵石
    时间:  2021年6月23日
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 捐赠

<center>
<img src="/res/img/myCode.png" width="80%" height="80%" />
</center>

如果觉得分享的内容不错，可以请作者喝杯咖啡.

---
